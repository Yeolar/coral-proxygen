/*
 *  Copyright (c) 2015, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

#include <proxygen/lib/http/codec/HTTPCodec.h>
#include <proxygen/lib/http/codec/experimental/HTTP2Framer.h>

#include <list>

namespace proxygen {

class HTTPTransaction;

class HTTP2PriorityQueue {

 private:
  class Node;

 public:

  typedef Node* Handle;

 private:

  class Node {
   public:
    Node(Node* inParent, HTTPCodec::StreamID id,
         uint32_t weight, HTTPTransaction *txn)
      : parent_(inParent),
        id_(id),
        weight_(weight),
        txn_(txn) {
    }

    Handle addChild(std::unique_ptr<Node> child) {
      child->parent_ = this;
      totalChildWeight_ += child->weight_;
      Node* raw = child.get();
      children_.push_back(std::move(child));
      raw->self_ = lastChild();
      return raw;
    }

    void addChildren(std::list<std::unique_ptr<Node>>&& children) {
      std::list<std::unique_ptr<Node>> emptyChilden;
      for (auto& child: children) {
        addChild(std::move(child));
      }
      std::swap(children, emptyChilden);
    }

    Node* parent() const {
      return parent_;
    }

    HTTPCodec::StreamID getID() const {
      return id_;
    }

    HTTPCodec::StreamID parentID() const {
      if (parent_) {
        return parent_->id_;
      }
      return 0;
    }

    HTTPTransaction* getTransaction() const {
      return txn_;
    }

    std::list<std::unique_ptr<Node>>::iterator lastChild() {
      auto it = children_.rbegin();
      it++;
      std::list<std::unique_ptr<Node>>::iterator result = it.base();
      return result;
    }

    // Add a new node as a child of this node
    Handle emplaceNode(std::unique_ptr<Node> node, bool exclusive) {
      std::list<std::unique_ptr<Node>> children;
      if (exclusive) {
        // this->children become new node's children
        std::swap(children, children_);
        totalChildWeight_ = 0;
        totalEnqueuedWeight_ = 0;
      }
      node->addChildren(std::move(children));
      auto res = addChild(std::move(node));
      return res;
    }

    std::unique_ptr<Node> detachChild(HTTPCodec::StreamID id) {
      auto it = std::find_if(children_.begin(), children_.end(),
                             [id] (const std::unique_ptr<Node>& n) {
                               return n->id_ == id; });
      if (it == children_.end()) {
        CHECK(false) << "Detach non-child id=" << id;
      }
      return detachChild(it->get());
    }

    std::unique_ptr<Node> detachChild(Node* node) {
      totalChildWeight_ -= node->weight_;
      auto it = node->self_;
      auto res = std::move(*node->self_);
      children_.erase(it);
      return res;
    }

    Handle reparent(Node* newParent, bool exclusive) {
      Node* oldParent = parent_;
      auto self = parent_->detachChild(this);
      parent_ = newParent;
      auto result = newParent->emplaceNode(std::move(self), exclusive);
      return this;
    }

    // Returns true if this is a descendant of node
    bool isDescendantOf(Node *node) {
      Node* cur = parent_;
      while (cur) {
        if (cur->id_ == node->id_) {
          return true;
        }
        cur = cur->parent_;
      }
      return false;
    }

    // True if this Node is in the egress queue
    bool isEnqueued() const {
      return (txn_ != nullptr && enqueued_);
    }

    void signalPendingEgress() {
      enqueued_ = true;
    }

    void clearPendingEgress() {
      CHECK(enqueued_);
      enqueued_ = false;
    }

    // Set a new weight for this node
    void updateWeight(uint8_t weight) {
      int16_t delta = weight - weight_;
      weight_ = weight;
      parent_->totalChildWeight_ += delta;
    }

    // Removes the node from the tree
    void removeFromTree() {
      // move my children to my parent
      parent_->addChildren(std::move(children_));
      (void)parent_->detachChild(this);
    }

    // Find the node for the given stream ID in the priority tree
    Node* findInTree(HTTPCodec::StreamID id) {
      if (id_ == id) {
        return this;
      }
      Node* res = nullptr;
      for (auto& child: children_) {
        res = child->findInTree(id);
        if (res) {
          break;
        }
      }
      return res;
    }

    double getRelativeWeight() const {
      if (!parent_) {
        return 1.0;
      }

      return (double)weight_ / parent_->totalChildWeight_;
    }

    double getRelativeEnqueuedWeight() const {
      if (!parent_) {
        return 1.0;
      }

      return (double)weight_ / parent_->totalEnqueuedWeight_;
    }

    /* Execute the given function on this node and all child nodes presently
     * enqueued, until one of them asks to stop, or the stop function returns
     * true.
     *
     * The all parameter visits every node, even the ones not currently
     * enqueued.
     *
     * The arguments to the function are
     *   txn - HTTPTransaction for the node
     *   ratio - weight of this txn relative to all peers (not just enequeued)
     */
    bool iterate(const std::function<bool(HTTPCodec::StreamID,
                                          HTTPTransaction *, double)>& fn,
                 const std::function<bool()>& stopFn, bool all) {
      bool stop = false;
      if (stopFn()) {
        return true;
      }
      if (parent_ /* exclude root */  && (all || isEnqueued())) {
        stop = fn(id_, txn_, getRelativeWeight());
      }
      for (auto& child: children_) {
        if (stop || stopFn()) {
          return true;
        }
        stop = child->iterate(fn, stopFn, all);
      }
      return stop;
    }

    typedef std::list<std::pair<HTTPCodec::StreamID, double>> PendingList;
    bool visitBFS(double relativeParentWeight,
                  const std::function<bool(HTTPCodec::StreamID,
                                          HTTPTransaction *, double)>& fn,
                  bool all,
                  PendingList& pendingNodes) {
      bool stop = false;
      if (parent_ != nullptr && (all || isEnqueued())) {
        stop = fn(id_, txn_,
                  relativeParentWeight * getRelativeEnqueuedWeight());
        if (!all || stop) {
          return stop;
        }
      }
      if (!all && totalEnqueuedWeight_ == 0) {
        return stop;
      }
      for (auto& child: children_) {
        pendingNodes.push_back({child->id_,
              relativeParentWeight * getRelativeEnqueuedWeight()});
      }
      return false;
    }

    void updateEnqueuedWeight() {
      totalEnqueuedWeight_ = totalChildWeight_;
      for (auto& child: children_) {
        child->updateEnqueuedWeight();
      }
      if (totalEnqueuedWeight_ == 0 && !isEnqueued()) {
        parent_->totalEnqueuedWeight_ -= weight_;
      }
    }

   private:
    Node *parent_{nullptr};
    HTTPCodec::StreamID id_{0};
    uint8_t weight_{16};
    HTTPTransaction *txn_{nullptr};
    bool enqueued_{false};
    uint64_t totalEnqueuedWeight_{0};
    uint64_t totalChildWeight_{0};
    std::list<std::unique_ptr<Node>> children_;
    std::list<std::unique_ptr<Node>>::iterator self_;
  };

 public:
  HTTP2PriorityQueue() {}

  // Find the node in priority tree
  Node* find(HTTPCodec::StreamID id) {
    if (id == 0) {
      return nullptr;
    }
    return root_.findInTree(id);
  }

  // Notify the queue when a transaction has egress
  void signalPendingEgress(Handle h) {
    if (!h->isEnqueued()) {
      h->signalPendingEgress();
      activeCount_++;
    }
  }

  // Notify the queue when a transaction no longer has egress
  void clearPendingEgress(Handle h) {
    CHECK(activeCount_ > 0);
    // clear does a CHECK on h->isEnqueued()
    h->clearPendingEgress();
    activeCount_--;
  }

  // adds new transaction (possibly nullptr) to the priority tree
  Handle addTransaction(HTTPCodec::StreamID id, http2::PriorityUpdate pri,
                        HTTPTransaction *txn) {
    CHECK(id != 0);

    Node* parent = &root_;
    if (pri.streamDependency != 0) {
      Node* dep = find(pri.streamDependency);
      if (dep == nullptr) {
        // specified a missing parent (timed out an idle node)?
        LOG(INFO) << "assigning default priority to txn=" << id;
      } else {
        parent = dep;
      }
    }
    auto node = coral::make_unique<Node>(parent, id, pri.weight, txn);
    auto result = parent->emplaceNode(std::move(node), pri.exclusive);
    return result;
  }

  // update the priority of an existing node
  Handle updatePriority(Handle handle, http2::PriorityUpdate pri) {
    Node* node = handle;
    node->updateWeight(pri.weight);
    if (pri.streamDependency == node->parentID() && !pri.exclusive) {
      // no move
      return handle;
    }

    Node* newParent = find(pri.streamDependency);
    if (!newParent) {
      newParent = &root_;
    }

    if (newParent->isDescendantOf(node)) {
      newParent = newParent->reparent(node->parent(), false);
    }
    return node->reparent(newParent, pri.exclusive);
  }

  // Remove the transaction from the priority tree
  void removeTransaction(Handle handle) {
    Node* node = handle;
    // TODO: or require the node to do it?
    if (node->isEnqueued()) {
      clearPendingEgress(handle);
    }
    node->removeFromTree();
  }

  // Returns true if there are no transaction with pending egress
  bool empty() const {
    return activeCount_ == 0;
  }

  // The number with pending egress
  uint64_t numPendingEgress() const {
    return activeCount_;
  }

  void iterate(const std::function<bool(HTTPCodec::StreamID,
                                        HTTPTransaction *, double)>& fn,
               const std::function<bool()>& stopFn, bool all) {
    root_.iterate(fn, stopFn, all);
  }

  void iterateBFS(const std::function<bool(HTTPCodec::StreamID,
                                           HTTPTransaction *, double)>& fn,
                  const std::function<bool()>& stopFn, bool all) {
    Node::PendingList pendingNodes{{0, 1.0}};
    bool stop = false;
    while (!stop && !stopFn() && !pendingNodes.empty()) {
      Node* node = findInternal(pendingNodes.front().first);
      if (node) {
        stop = node->visitBFS(pendingNodes.front().second, fn, all,
                              pendingNodes);
      }
      pendingNodes.pop_front();
    }
  }

  struct WeightCmp {
    bool operator()(const std::pair<HTTPTransaction*, double>& t1,
                    const std::pair<HTTPTransaction*, double>& t2) {
      return t1.second > t2.second;
    }
  };


  std::list<std::pair<HTTPTransaction*, double>> nextEgress() {
    std::list<std::pair<HTTPTransaction*, double>> result;
    auto fn = [&result] (HTTPCodec::StreamID id, HTTPTransaction* txn,
                         double r) {
      result.push_back({txn, r});
      return false;
    };

    root_.updateEnqueuedWeight();

    Node::PendingList pendingNodes{{0, 1.0}};
    bool stop = false;
    while (!stop && !pendingNodes.empty()) {
      Node* node = findInternal(pendingNodes.front().first);
      if (node) {
        stop = node->visitBFS(pendingNodes.front().second, fn, false,
                              pendingNodes);
      }
      pendingNodes.pop_front();
    }
    result.sort(WeightCmp());
    return result;
  }

 private:
  Node* findInternal(HTTPCodec::StreamID id) {
    if (id == 0) {
      return &root_;
    }
    return root_.findInTree(id);
  }

  Node root_{nullptr, 0, 1, nullptr};
  uint64_t activeCount_{0};
};


}
