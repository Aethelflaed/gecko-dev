/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_cache_CacheChild_h
#define mozilla_dom_cache_CacheChild_h

#include "mozilla/dom/cache/ActorChild.h"
#include "mozilla/dom/cache/PCacheChild.h"

namespace mozilla {
namespace dom {
namespace cache {

class Cache;

class CacheChild MOZ_FINAL : public PCacheChild
                           , public ActorChild
{
public:
  CacheChild();
  ~CacheChild();

  void SetListener(Cache* aListener);

  // Must be called by the associated Cache listener in its ActorDestroy()
  // method.  Also, Cache must Send__delete__() the actor in its destructor to
  // trigger ActorDestroy() if it has not been called yet.
  void ClearListener();

  // ActorChild methods

  // Synchronously call ActorDestroy on our Cache listener and then start the
  // actor destruction asynchronously from the parent-side.
  virtual void StartDestroy() MOZ_OVERRIDE;

private:
  // PCacheChild methods
  virtual void
  ActorDestroy(ActorDestroyReason aReason) MOZ_OVERRIDE;

  virtual bool
  RecvMatchResponse(const RequestId& requestId, const nsresult& aRv,
                    const PCacheResponseOrVoid& aResponse) MOZ_OVERRIDE;
  virtual bool
  RecvMatchAllResponse(const RequestId& requestId, const nsresult& aRv,
                       nsTArray<PCacheResponse>&& responses) MOZ_OVERRIDE;
  virtual bool
  RecvAddAllResponse(const RequestId& requestId,
                     const nsresult& aRv) MOZ_OVERRIDE;
  virtual bool
  RecvPutResponse(const RequestId& aRequestId,
                  const nsresult& aRv) MOZ_OVERRIDE;
  virtual bool
  RecvDeleteResponse(const RequestId& requestId, const nsresult& aRv,
                     const bool& result) MOZ_OVERRIDE;
  virtual bool
  RecvKeysResponse(const RequestId& requestId, const nsresult& aRv,
                   nsTArray<PCacheRequest>&& requests) MOZ_OVERRIDE;

  // Use a weak ref so actor does not hold DOM object alive past content use.
  // The Cache object must call ClearListener() to null this before its
  // destroyed.
  Cache* MOZ_NON_OWNING_REF mListener;

  NS_DECL_OWNINGTHREAD
};

} // namespace cache
} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_cache_CacheChild_h
