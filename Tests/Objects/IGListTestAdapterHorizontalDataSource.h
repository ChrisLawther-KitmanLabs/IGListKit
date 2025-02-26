/*
 * Copyright (c) Meta Platforms, Inc. and its affiliates.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#import <Foundation/Foundation.h>

#import <IGListKit/IGListAdapterDataSource.h>

@interface IGListTestAdapterHorizontalDataSource : NSObject <IGListAdapterDataSource>

// array of numbers which is then passed to -[IGListTestSection setItems:]
@property (nonatomic, copy) NSArray <NSNumber *> *objects;

@property (nonatomic, strong) UIView *backgroundView;

@end
