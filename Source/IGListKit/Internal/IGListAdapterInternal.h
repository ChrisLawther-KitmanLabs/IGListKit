/*
 * Copyright (c) Meta Platforms, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#import "IGListAdapter.h"
#import "IGListBatchContext.h"
#import "IGListCollectionContext.h"

#import "IGListAdapter+UICollectionView.h"
#import "IGListAdapterProxy.h"
#import "IGListDisplayHandler.h"
#import "IGListSectionMap.h"
#import "IGListWorkingRangeHandler.h"

NS_ASSUME_NONNULL_BEGIN

/// Generate a string representation of a reusable view class when registering with a UICollectionView.
NS_INLINE NSString *IGListReusableViewIdentifier(Class viewClass, NSString * _Nullable kind, NSString * _Nullable givenReuseIdentifier) {
    return [NSString stringWithFormat:@"%@%@%@", kind ?: @"", givenReuseIdentifier ?: @"", NSStringFromClass(viewClass)];
}

@interface IGListAdapter ()
<
IGListCollectionContext,
IGListBatchContext
>
{
    __weak UICollectionView *_collectionView;
    BOOL _isDequeuingCell;
    BOOL _isDequeuingSupplementaryView;

    BOOL _isSendingWorkingRangeDisplayUpdates;
}

@property (nonatomic, strong) id <IGListUpdatingDelegate> updater;

@property (nonatomic, strong, readonly) IGListSectionMap *sectionMap;
@property (nonatomic, strong, readonly) IGListDisplayHandler *displayHandler;
@property (nonatomic, strong, readonly) IGListWorkingRangeHandler *workingRangeHandler;

@property (nonatomic, strong, nullable) IGListAdapterProxy *delegateProxy;

@property (nonatomic, strong, nullable) UIView *emptyBackgroundView;

// We need to special case interactive section moves that are moved to the last position
@property (nonatomic) BOOL isLastInteractiveMoveToLastSectionIndex;

// We're in the middle of updating the objects.
@property (nonatomic) BOOL isInObjectUpdateTransaction;

/**
 When making object updates inside a batch update block, delete operations must use the section /before/ any moves take
 place. This includes when other objects are deleted or inserted ahead of the section controller making the mutations.
 In order to account for this we must track when the adapter is in the middle of an update block as well as the section
 controller mapping prior to the transition.

 Note that the previous section controller map is destroyed as soon as a transition is finished so there is no dangling
 objects or section controllers.
 */
@property (nonatomic, assign) BOOL isInUpdateBlock;
@property (nonatomic, strong, nullable) IGListSectionMap *previousSectionMap;

/**
 Set of cell identifiers registered with the list context.
 Identifiers are constructed with the `IGListReusableViewIdentifier` function.
 */
@property (nonatomic, strong) NSMutableSet<NSString *> *registeredCellIdentifiers;
@property (nonatomic, strong) NSMutableSet<NSString *> *registeredNibNames;
@property (nonatomic, strong) NSMutableSet<NSString *> *registeredSupplementaryViewIdentifiers;
@property (nonatomic, strong) NSMutableSet<NSString *> *registeredSupplementaryViewNibNames;

- (void)mapView:(__kindof UIView *)view toSectionController:(IGListSectionController *)sectionController;
- (nullable IGListSectionController *)sectionControllerForView:(__kindof UIView *)view;
- (void)removeMapForView:(__kindof UIView *)view;

- (NSArray *)indexPathsFromSectionController:(IGListSectionController *)sectionController
                                     indexes:(NSIndexSet *)indexes
                  usePreviousIfInUpdateBlock:(BOOL)usePreviousIfInUpdateBlock;

- (nullable NSIndexPath *)indexPathForSectionController:(IGListSectionController *)controller
                                                  index:(NSInteger)index
                             usePreviousIfInUpdateBlock:(BOOL)usePreviousIfInUpdateBlock;

@end

NS_ASSUME_NONNULL_END
