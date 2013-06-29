/*
 Copyright (c) 2012, Nicolas Seriot
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * Neither the name of the Nicolas Seriot nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//
//  STTwitterAPI.h
//  STTwitterRequests
//
//  Created by Nicolas Seriot on 9/18/12.
//  Copyright (c) 2012 Nicolas Seriot. All rights reserved.
//

#import <Foundation/Foundation.h>

/*
 Partial Objective-C front-end for https://dev.twitter.com/docs/api/1.1
 */

/*
 Tweet fields contents
 https://dev.twitter.com/docs/platform-objects/tweets
 https://dev.twitter.com/blog/new-withheld-content-fields-api-responses
 */

@interface STTwitterAPIWrapper : NSObject

#if TARGET_OS_IPHONE
#else
+ (STTwitterAPIWrapper *)twitterAPIWithOAuthOSX;
#endif

+ (STTwitterAPIWrapper *)twitterAPIWithOAuthConsumerName:(NSString *)consumerName
                                             consumerKey:(NSString *)consumerKey
                                          consumerSecret:(NSString *)consumerSecret;

+ (STTwitterAPIWrapper *)twitterAPIWithOAuthConsumerName:(NSString *)consumerName
                                             consumerKey:(NSString *)consumerKey
                                          consumerSecret:(NSString *)consumerSecret
                                                username:(NSString *)username
                                                password:(NSString *)password;

+ (STTwitterAPIWrapper *)twitterAPIWithOAuthConsumerName:(NSString *)consumerName
                                             consumerKey:(NSString *)consumerKey
                                          consumerSecret:(NSString *)consumerSecret
                                              oauthToken:(NSString *)oauthToken
                                        oauthTokenSecret:(NSString *)oauthTokenSecret;

// https://dev.twitter.com/docs/auth/application-only-auth
+ (STTwitterAPIWrapper *)twitterAPIApplicationOnlyWithConsumerKey:(NSString *)consumerKey
                                                   consumerSecret:(NSString *)consumerSecret;

- (void)postTokenRequest:(void(^)(NSURL *url, NSString *oauthToken))successBlock
           oauthCallback:(NSString *)oauthCallback
              errorBlock:(void(^)(NSError *error))errorBlock;

- (void)postAccessTokenRequestWithPIN:(NSString *)pin
                         successBlock:(void(^)(NSString *oauthToken, NSString *oauthTokenSecret, NSString *userID, NSString *screenName))successBlock
                           errorBlock:(void(^)(NSError *error))errorBlock;

- (void)verifyCredentialsWithSuccessBlock:(void(^)(NSString *username))successBlock errorBlock:(void(^)(NSError *error))errorBlock;

- (void)invalidateBearerTokenWithSuccessBlock:(void(^)())successBlock
                                   errorBlock:(void(^)(NSError *error))errorBlock;

@property (nonatomic, retain) NSString *consumerName;
@property (nonatomic, retain) NSString *userName; // available for osx, set after successful connection for STTwitterOAuth

@property (nonatomic, readonly) NSString *oauthAccessToken;
@property (nonatomic, readonly) NSString *oauthAccessTokenSecret;
@property (nonatomic, readonly) NSString *bearerToken;

#pragma mark Generic methods to GET and POST

- (void)getResource:(NSString *)resource
         parameters:(NSDictionary *)parameters
       successBlock:(void(^)(id json))successBlock
         errorBlock:(void(^)(NSError *error))errorBlock;

- (void)postResource:(NSString *)resource
          parameters:(NSDictionary *)parameters
        successBlock:(void(^)(id response))successBlock
          errorBlock:(void(^)(NSError *error))errorBlock;

#pragma mark Timelines

/*
 GET	statuses/mentions_timeline
 Returns Tweets (*: mentions for the user)
 
 Returns the 20 most recent mentions (tweets containing a users's @screen_name) for the authenticating user.
 
 The timeline returned is the equivalent of the one seen when you view your mentions on twitter.com.
 
 This method can only return up to 800 tweets.
 */

- (void)getMentionsTimelineSinceID:(NSString *)optionalSinceID
							 count:(NSUInteger)optionalCount
					  successBlock:(void(^)(NSArray *statuses))successBlock
						errorBlock:(void(^)(NSError *error))errorBlock;

/*
 GET	statuses/user_timeline
 Returns Tweets (*: tweets for the user)
 
 Returns a collection of the most recent Tweets posted by the user indicated by the screen_name or user_id parameters.
 
 User timelines belonging to protected users may only be requested when the authenticated user either "owns" the timeline or is an approved follower of the owner.
 
 The timeline returned is the equivalent of the one seen when you view a user's profile on twitter.com.
 
 This method can only return up to 3,200 of a user's most recent Tweets. Native retweets of other statuses by the user is included in this total, regardless of whether include_rts is set to false when requesting this resource.
 */

- (void)getUserTimelineWithScreenName:(NSString *)screenName
                              sinceID:(NSString *)optionalSinceID
                                maxID:(NSString *)optionalMaxID
								count:(NSUInteger)optionalCount
                         successBlock:(void(^)(NSArray *statuses))successBlock
                           errorBlock:(void(^)(NSError *error))errorBlock;

- (void)getUserTimelineWithScreenName:(NSString *)screenName
								count:(NSUInteger)optionalCount
                         successBlock:(void(^)(NSArray *statuses))successBlock
                           errorBlock:(void(^)(NSError *error))errorBlock;

- (void)getUserTimelineWithScreenName:(NSString *)screenName
                         successBlock:(void(^)(NSArray *statuses))successBlock
                           errorBlock:(void(^)(NSError *error))errorBlock;

/*
 GET	statuses/home_timeline
 Returns Tweets (*: tweets from people the user follows)
 
 Returns a collection of the most recent Tweets and retweets posted by the authenticating user and the users they follow. The home timeline is central to how most users interact with the Twitter service.
 
 Up to 800 Tweets are obtainable on the home timeline. It is more volatile for users that follow many users or follow users who tweet frequently.
 */

- (void)getHomeTimelineSinceID:(NSString *)optionalSinceID
                         count:(NSUInteger)optionalCount
                  successBlock:(void(^)(NSArray *statuses))successBlock
                    errorBlock:(void(^)(NSError *error))errorBlock;

/*
 GET    statuses/retweets_of_me
 
 Returns the most recent tweets authored by the authenticating user that have been retweeted by others. This timeline is a subset of the user's GET statuses/user_timeline. See Working with Timelines for instructions on traversing timelines.
 */

- (void)getStatusesRetweetsOfMeWithOptionalCount:(NSString *)count
                                 optionalSinceID:(NSString *)sinceID
                                   optionalMaxID:(NSString *)maxID
                                        trimUser:(BOOL)trimUser
                                 includeEntitied:(BOOL)includeEntities
                             includeUserEntities:(BOOL)includeUserEntities
                                    successBlock:(void(^)(NSArray *statuses))successBlock
                                      errorBlock:(void(^)(NSError *error))errorBlock;

// convenience method without all optional values
- (void)getStatusesRetweetsOfMeWithSuccessBlock:(void(^)(NSArray *statuses))successBlock
                                     errorBlock:(void(^)(NSError *error))errorBlock;

#pragma mark Tweets

//	GET		statuses/retweets/:id
- (void)getStatusesRetweetsForID:(NSString *)statusID
                   optionalCount:(NSString *)count
                        trimUser:(BOOL)trimUser
                    successBlock:(void(^)(NSArray *statuses))successBlock
                      errorBlock:(void(^)(NSError *error))errorBlock;

//	GET		statuses/show/:id
- (void)getStatusesShowID:(NSString *)statusID
                 trimUser:(BOOL)trimUser
         includeMyRetweet:(BOOL)includeMyRetweet
          includeEntities:(BOOL)includeEntities
             successBlock:(void(^)(NSDictionary *status))successBlock
               errorBlock:(void(^)(NSError *error))errorBlock;

//	POST	statuses/destroy/:id
//	Returns Tweets (1: the destroyed tweet)
- (void)postDestroyStatusWithID:(NSString *)statusID
                   successBlock:(void(^)(NSDictionary *status))successBlock
                     errorBlock:(void(^)(NSError *error))errorBlock;

//	POST	statuses/update
//	Returns Tweets (1: the new tweet)
- (void)postStatusUpdate:(NSString *)status
       inReplyToStatusID:(NSString *)optionalExistingStatusID
                 placeID:(NSString *)optionalPlaceID // wins over lat/lon
                     lat:(NSString *)optionalLat
                     lon:(NSString *)optionalLon
            successBlock:(void(^)(NSDictionary *status))successBlock
              errorBlock:(void(^)(NSError *error))errorBlock;

//	POST	statuses/retweet/:id
//	Returns Tweets (1: the retweeted tweet)
- (void)postStatusRetweetWithID:(NSString *)statusID
                   successBlock:(void(^)(NSDictionary *status))successBlock
                     errorBlock:(void(^)(NSError *error))errorBlock;

//	POST	statuses/update_with_media
//	Returns Tweets (1: the new tweet)
- (void)postStatusUpdate:(NSString *)status
       inReplyToStatusID:(NSString *)optionalExistingStatusID
                mediaURL:(NSURL *)mediaURL
                 placeID:(NSString *)optionalPlaceID // wins over lat/lon
                     lat:(NSString *)optionalLat
                     lon:(NSString *)optionalLon
            successBlock:(void(^)(NSDictionary *status))successBlock
              errorBlock:(void(^)(NSError *error))errorBlock;

//	GET		statuses/oembed

//  GET     statuses/retweeters/ids
- (void)getStatusesRetweetersIDsForStatusID:(NSString *)statusID
                             optionalCursor:(NSString *)cursor
                         returnIDsAsStrings:(BOOL)returnIDsAsStrings
                               successBlock:(void(^)(NSArray *ids, NSString *previousCursor, NSString *nextCursor))successBlock
                                 errorBlock:(void(^)(NSError *error))errorBlock;

#pragma mark Search

//	GET		search/tweets

// full method
- (void)getSearchTweetsWithQuery:(NSString *)q
                 optionalGeocode:(NSString *)geoCode // eg. "37.781157,-122.398720,1mi"
                    optionalLang:(NSString *)lang // eg. "eu"
                  optionalLocale:(NSString *)locale // eg. "ja"
              optionalResultType:(NSString *)resultType // eg. "mixed, recent, popular"
                   optionalCount:(NSString *)count // eg. "100"
                   optionalUntil:(NSString *)until // eg. "2012-09-01"
                 optionalSinceID:(NSString *)sinceID // eg. "12345"
                   optionalMaxID:(NSString *)maxID // eg. "54321"
                 includeEntities:(BOOL)includeEntities
                optionalCallback:(NSString *)callback // eg. "processTweets"
					successBlock:(void(^)(NSDictionary *searchMetadata, NSArray *statuses))successBlock
					  errorBlock:(void(^)(NSError *error))errorBlock;

// convenience method
//	Returns Tweets (*: tweets matching the query)
- (void)getSearchTweetsWithQuery:(NSString *)q
					successBlock:(void(^)(NSDictionary *searchMetadata, NSArray *statuses))successBlock
                      errorBlock:(void(^)(NSError *error))errorBlock;

#pragma mark Streaming

//	POST	statuses/filter

//	GET		statuses/sample

//	GET		statuses/firehose

//	GET		user

//	GET		site

#pragma mark Direct Messages

//	GET		direct_messages
//	Returns Tweets (*: direct messages to the user)
- (void)getDirectMessagesSinceID:(NSString *)optionalSinceID
						   count:(NSUInteger)optionalCount
					successBlock:(void(^)(NSArray *statuses))successBlock
					  errorBlock:(void(^)(NSError *error))errorBlock;

//	GET		direct_messages/sent

//	GET		direct_messages/show

//	POST	direct_messages/destroy
//	Returns Tweets (1: the destroyed DM)
- (void)postDestroyDirectMessageWithID:(NSString *)dmID
						  successBlock:(void(^)(NSDictionary *dm))successBlock
							errorBlock:(void(^)(NSError *error))errorBlock;

//	POST	direct_messages/new
//	Returns Tweets (1: the sent DM)
- (void)postDirectMessage:(NSString *)status
					   to:(NSString *)screenName
             successBlock:(void(^)(NSDictionary *dm))successBlock
               errorBlock:(void(^)(NSError *error))errorBlock;

#pragma mark Friends & Followers

//	GET		friends/ids
//	Returns Users (*: user IDs for followees)
- (void)getFriendsIDsForScreenName:(NSString *)screenName
				      successBlock:(void(^)(NSArray *friends))successBlock
                        errorBlock:(void(^)(NSError *error))errorBlock;

//	GET		followers/ids
//	Returns Users (*: user IDs for followers)
- (void)getFollowersIDsForScreenName:(NSString *)screenName
                        successBlock:(void(^)(NSArray *followers))successBlock
                          errorBlock:(void(^)(NSError *error))errorBlock;

//	GET		friendships/lookup

//	GET		friendships/incoming

//	GET		friendships/outgoing

//	POST	friendships/create
//	Returns Users (1: the followed user)
- (void)postFollow:(NSString *)screenName
	  successBlock:(void(^)(NSDictionary *user))successBlock
		errorBlock:(void(^)(NSError *error))errorBlock;

//	POST	friendships/destroy
//	Returns Users (1: the unfollowed user)
- (void)postUnfollow:(NSString *)screenName
		successBlock:(void(^)(NSDictionary *user))successBlock
		  errorBlock:(void(^)(NSError *error))errorBlock;

//	POST	friendships/update
//	Returns ?
- (void)postUpdateNotifications:(BOOL)notify
				  forScreenName:(NSString *)screenName
				   successBlock:(void(^)(NSDictionary *relationship))successBlock
					 errorBlock:(void(^)(NSError *error))errorBlock;

//	GET		friendships/show

//	GET		friends/list
- (void)getFriendsForScreenName:(NSString *)screenName
				   successBlock:(void(^)(NSArray *friends))successBlock
                     errorBlock:(void(^)(NSError *error))errorBlock;

//	GET		followers/list
- (void)getFollowersForScreenName:(NSString *)screenName
					 successBlock:(void(^)(NSArray *followers))successBlock
                       errorBlock:(void(^)(NSError *error))errorBlock;

#pragma mark Users

//	GET		account/settings

//	GET		account/verify_credentials
//	Returns Users (1: the user)
- (void)getAccountVerifyCredentialsSkipStatus:(BOOL)skipStatus
                                 successBlock:(void(^)(NSDictionary *myInfo))successBlock
                                   errorBlock:(void(^)(NSError *error))errorBlock;

//	POST	account/settings

//	POST	account/update_delivery_device

//	POST	account/update_profile
//	Returns Users (1: the user)
- (void)postUpdateProfile:(NSDictionary *)profileData
			 successBlock:(void(^)(NSDictionary *myInfo))successBlock
			   errorBlock:(void(^)(NSError *error))errorBlock;

//	POST	account/update_profile_background_image

//	POST	account/update_profile_colors

//	POST	account/update_profile_image
//	Returns Users (1: the user)
#if TARGET_OS_IPHONE
- (void)postUpdateProfileImage:(UIImage *)newImage
#else
- (void)postUpdateProfileImage:(NSImage *)newImage
#endif
				  successBlock:(void(^)(NSDictionary *myInfo))successBlock
					errorBlock:(void(^)(NSError *error))errorBlock;

//	GET		blocks/list

//	GET		blocks/ids

//	POST	blocks/create

//	POST	blocks/destroy

//	GET		users/lookup

//	GET		users/show
//	Returns Users (1: detailed information for the user)
- (void)getUserInformationFor:(NSString *)screenName
				 successBlock:(void(^)(NSDictionary *user))successBlock
				   errorBlock:(void(^)(NSError *error))errorBlock;

- (void)profileImageFor:(NSString *)screenName
#if TARGET_OS_IPHONE
           successBlock:(void(^)(UIImage *image))successBlock
#else
           successBlock:(void(^)(NSImage *image))successBlock
#endif
             errorBlock:(void(^)(NSError *error))errorBlock;

//	GET		users/search

- (void)getUsersSearchQuery:(NSString *)query
               optionalPage:(NSString *)page
              optionalCount:(NSString *)count
            includeEntities:(BOOL)includeEntities
               successBlock:(void(^)(NSDictionary *users))successBlock
                 errorBlock:(void(^)(NSError *error))errorBlock;

//	GET		users/contributees

//	GET		users/contributors

#pragma mark Suggested Users

//	GET		users/suggestions/:slug

//	GET		users/suggestions

//	GET		users/suggestions/:slug/members

#pragma mark Favorites

//	GET		favorites/list
//	Returns Tweets (20: last 20 favorited tweets)
- (void)getFavoritesListWithSuccessBlock:(void(^)(NSArray *statuses))successBlock
                              errorBlock:(void(^)(NSError *error))errorBlock;

//	POST	favorites/destroy
//	POST	favorites/create
//	Returns Tweets (1: the (un)favorited tweet)
- (void)postFavoriteState:(BOOL)favoriteState
              forStatusID:(NSString *)statusID
             successBlock:(void(^)(NSDictionary *status))successBlock
               errorBlock:(void(^)(NSError *error))errorBlock;

#pragma mark Lists

/*
 GET    lists/list
 
 Returns all lists the authenticating or specified user subscribes to, including their own. The user is specified using the user_id or screen_name parameters. If no user is given, the authenticating user is used.
 
 This method used to be GET lists in version 1.0 of the API and has been renamed for consistency with other call.
 
 A maximum of 100 results will be returned by this call. Subscribed lists are returned first, followed by owned lists. This means that if a user subscribes to 90 lists and owns 20 lists, this method returns 90 subscriptions and 10 owned lists. The reverse method returns owned lists first, so with reverse=true, 20 owned lists and 80 subscriptions would be returned. If your goal is to obtain every list a user owns or subscribes to, use GET lists/ownerships and/or GET lists/subscriptions instead.
 */

- (void)getListsSubscribedByUsername:(NSString *)username
                            orUserID:(NSString *)userID
                             reverse:(BOOL)reverse
                        successBlock:(void(^)(NSArray *lists))successBlock
                          errorBlock:(void(^)(NSError *error))errorBlock;

/*
 GET	lists/statuses
 
 Returns a timeline of tweets authored by members of the specified list. Retweets are included by default. Use the include_rts=false parameter to omit retweets. Embedded Timelines is a great way to embed list timelines on your website.
 */

- (void)getListsStatusesForListID:(NSString *)listID
                  optionalSinceID:(NSString *)sinceID
                    optionalMaxID:(NSString *)maxID
                    optionalCount:(NSString *)count
                  includeEntities:(BOOL)includeEntities
                  includeRetweets:(BOOL)includeRetweets
                     successBlock:(void(^)(NSArray *statuses))successBlock
                       errorBlock:(void(^)(NSError *error))errorBlock;

- (void)getListsStatusesForSlug:(NSString *)slug
                ownerScreenName:(NSString *)ownerScreenName
                      orOwnerID:(NSString *)ownerID
                optionalSinceID:(NSString *)sinceID
                  optionalMaxID:(NSString *)maxID
                  optionalCount:(NSString *)count
                includeEntities:(BOOL)includeEntities
                includeRetweets:(BOOL)includeRetweets
                   successBlock:(void(^)(NSArray *statuses))successBlock
                     errorBlock:(void(^)(NSError *error))errorBlock;

/*
 POST	lists/members/destroy
 
 Removes the specified member from the list. The authenticated user must be the list's owner to remove members from the list.
 */

- (void)postListsMembersDestroyForListID:(NSString *)listID
                            successBlock:(void(^)())successBlock
                              errorBlock:(void(^)(NSError *error))errorBlock;

- (void)postListsMembersDestroyForSlug:(NSString *)slug
                        optionalUserID:(NSString *)userID
                    optionalScreenName:(NSString *)screenName
               optionalOwnerScreenName:(NSString *)ownerScreenName
                       optionalOwnerID:(NSString *)ownerID
                          successBlock:(void(^)())successBlock
                            errorBlock:(void(^)(NSError *error))errorBlock;

/*
 GET	lists/subscribers
 
 Returns the subscribers of the specified list. Private list subscribers will only be shown if the authenticated user owns the specified list.
 */

- (void)getListsSubscribersForSlug:(NSString *)slug
                   ownerScreenName:(NSString *)ownerScreenName
                         orOwnerID:(NSString *)ownerID
                    optionalCursor:(NSString *)cursor
                   includeEntities:(BOOL)includeEntities
                        skipStatus:(BOOL)skipStatus
                      successBlock:(void(^)())successBlock
                        errorBlock:(void(^)(NSError *error))errorBlock;

- (void)getListsSubscribersForListID:(NSString *)listID
                      optionalCursor:(NSString *)cursor
                     includeEntities:(BOOL)includeEntities
                          skipStatus:(BOOL)skipStatus
                        successBlock:(void(^)())successBlock
                          errorBlock:(void(^)(NSError *error))errorBlock;

/*
 POST	lists/subscribers/create
 
 Subscribes the authenticated user to the specified list.
 */

- (void)postListSubscribersCreateForListID:(NSString *)listID
                              successBlock:(void(^)())successBlock
                                errorBlock:(void(^)(NSError *error))errorBlock;

- (void)postListSubscribersCreateForSlug:(NSString *)slug
                         ownerScreenName:(NSString *)ownerScreenName
                               orOwnerID:(NSString *)ownerID
                            successBlock:(void(^)())successBlock
                              errorBlock:(void(^)(NSError *error))errorBlock;

/*
 GET	lists/subscribers/show
 
 Check if the specified user is a subscriber of the specified list. Returns the user if they are subscriber.
 */

- (void)getListsSubscribersShowForListID:(NSString *)listID
                                  userID:(NSString *)userID
                            orScreenName:(NSString *)screenName
                         includeEntities:(BOOL)includeEntities
                              skipStatus:(BOOL)skipStatus
                            successBlock:(void(^)())successBlock
                              errorBlock:(void(^)(NSError *error))errorBlock;

- (void)getListsSubscribersShowForSlug:(NSString *)slug
                       ownerScreenName:(NSString *)ownerScreenName
                             orOwnerID:(NSString *)ownerID
                                userID:(NSString *)userID
                          orScreenName:(NSString *)screenName
                       includeEntities:(BOOL)includeEntities
                            skipStatus:(BOOL)skipStatus
                          successBlock:(void(^)())successBlock
                            errorBlock:(void(^)(NSError *error))errorBlock;

/*
 POST	lists/subscribers/destroy
 
 Unsubscribes the authenticated user from the specified list.
 */

- (void)postListSubscribersDestroyForListID:(NSString *)listID
                               successBlock:(void(^)())successBlock
                                 errorBlock:(void(^)(NSError *error))errorBlock;

- (void)postListSubscribersDestroyForSlug:(NSString *)slug
                          ownerScreenName:(NSString *)ownerScreenName
                                orOwnerID:(NSString *)ownerID
                             successBlock:(void(^)())successBlock
                               errorBlock:(void(^)(NSError *error))errorBlock;

/*
 POST	lists/members/create_all
 
 Adds multiple members to a list, by specifying a comma-separated list of member ids or screen names. The authenticated user must own the list to be able to add members to it. Note that lists can't have more than 5,000 members, and you are limited to adding up to 100 members to a list at a time with this method.
 
 Please note that there can be issues with lists that rapidly remove and add memberships. Take care when using these methods such that you are not too rapidly switching between removals and adds on the same list.
 */

- (void)postListsMembersCreateAllForListID:(NSString *)listID
                                   userIDs:(NSArray *)userIDs // array of strings
                             orScreenNames:(NSArray *)screenNames // array of strings
                              successBlock:(void(^)())successBlock
                                errorBlock:(void(^)(NSError *error))errorBlock;

- (void)postListsMembersCreateAllForSlug:(NSString *)slug
                         ownerScreenName:(NSString *)ownerScreenName
                               orOwnerID:(NSString *)ownerID
                                 userIDs:(NSArray *)userIDs // array of strings
                           orScreenNames:(NSArray *)screenNames // array of strings
                            successBlock:(void(^)())successBlock
                              errorBlock:(void(^)(NSError *error))errorBlock;

/*
 GET	lists/members/show
 
 Check if the specified user is a member of the specified list.
 */

- (void)getListsMembersShowForListID:(NSString *)listID
                              userID:(NSString *)userID
                          screenName:(NSString *)screenName
                     includeEntities:(BOOL)includeEntities
                          skipStatus:(BOOL)skipStatus
                        successBlock:(void(^)(NSDictionary *user))successBlock
                          errorBlock:(void(^)(NSError *error))errorBlock;

- (void)getListsMembersShowForSlug:(NSString *)slug
                   ownerScreenName:(NSString *)ownerScreenName
                         orOwnerID:(NSString *)ownerID
                            userID:(NSString *)userID
                        screenName:(NSString *)screenName
                   includeEntities:(BOOL)includeEntities
                        skipStatus:(BOOL)skipStatus
                      successBlock:(void(^)(NSDictionary *user))successBlock
                        errorBlock:(void(^)(NSError *error))errorBlock;

/*
 GET		lists/members
 
 Returns the members of the specified list. Private list members will only be shown if the authenticated user owns the specified list.
 */

- (void)getListsMembersForListID:(NSString *)listID
                  optionalCursor:(NSString *)cursor
                 includeEntities:(BOOL)includeEntities
                      skipStatus:(BOOL)skipStatus
                    successBlock:(void(^)(NSArray *users, NSString *previousCursor, NSString *nextCursor))successBlock
                      errorBlock:(void(^)(NSError *error))errorBlock;

- (void)getListsMembersForSlug:(NSString *)slug
               ownerScreenName:(NSString *)screenName
                     orOwnerID:(NSString *)ownerID
                optionalCursor:(NSString *)cursor
               includeEntities:(BOOL)includeEntities
                    skipStatus:(BOOL)skipStatus
                  successBlock:(void(^)(NSArray *users, NSString *previousCursor, NSString *nextCursor))successBlock
                    errorBlock:(void(^)(NSError *error))errorBlock;

/*
 POST	lists/members/create
 
 Creates a new list for the authenticated user. Note that you can't create more than 20 lists per account.
 */

- (void)postListMemberCreateForListID:(NSString *)listID
                               userID:(NSString *)userID
                           screenName:(NSString *)screenName
                         successBlock:(void(^)())successBlock
                           errorBlock:(void(^)(NSError *error))errorBlock;

- (void)postListMemberCreateForSlug:(NSString *)slug
                    ownerScreenName:(NSString *)ownerScreenName
                          orOwnerID:(NSString *)ownerID
                             userID:(NSString *)userID
                         screenName:(NSString *)screenName
                       successBlock:(void(^)())successBlock
                         errorBlock:(void(^)(NSError *error))errorBlock;

/*
 POST	lists/destroy
 
 Deletes the specified list. The authenticated user must own the list to be able to destroy it.
 */

- (void)postListsDestroyForListID:(NSString *)listID
                     successBlock:(void(^)())successBlock
                       errorBlock:(void(^)(NSError *error))errorBlock;

- (void)postListsDestroyForSlug:(NSString *)slug
                ownerScreenName:(NSString *)ownerScreenName
                      orOwnerID:(NSString *)ownerID
                   successBlock:(void(^)())successBlock
                     errorBlock:(void(^)(NSError *error))errorBlock;

/*
 POST	lists/update
 Updates the specified list. The authenticated user must own the list to be able to update it.
 */

- (void)postListsUpdateForListID:(NSString *)listID
                    optionalName:(NSString *)name
                       isPrivate:(BOOL)isPrivate
             optionalDescription:(NSString *)description
                    successBlock:(void(^)())successBlock
                      errorBlock:(void(^)(NSError *error))errorBlock;

- (void)postListsUpdateForSlug:(NSString *)slug
               ownerScreenName:(NSString *)ownerScreenName
                     orOwnerID:(NSString *)ownerID
                  optionalName:(NSString *)name
                     isPrivate:(BOOL)isPrivate
           optionalDescription:(NSString *)description
                  successBlock:(void(^)())successBlock
                    errorBlock:(void(^)(NSError *error))errorBlock;

/*
 POST	lists/create
 
 Creates a new list for the authenticated user. Note that you can't create more than 20 lists per account.
 */

- (void)postListsCreateWithName:(NSString *)name
                      isPrivate:(BOOL)isPrivate
            optionalDescription:(NSString *)description
                   successBlock:(void(^)(NSDictionary *list))successBlock
                     errorBlock:(void(^)(NSError *error))errorBlock;

/*
 GET	lists/show
 
 Returns the specified list. Private lists will only be shown if the authenticated user owns the specified list.
 */

- (void)getListsShowListID:(NSString *)listID
              successBlock:(void(^)(NSDictionary *list))successBlock
                errorBlock:(void(^)(NSError *error))errorBlock;

- (void)getListsShowListSlug:(NSString *)slug
             ownerScreenName:(NSString *)ownerScreenName
                   orOwnerID:(NSString *)ownerID
                successBlock:(void(^)(NSDictionary *list))successBlock
                  errorBlock:(void(^)(NSError *error))errorBlock;

/*
 GET	lists/subscriptions
 
 Obtain a collection of the lists the specified user is subscribed to, 20 lists per page by default. Does not include the user's own lists.
 */

- (void)getListsSubscriptionsForUserID:(NSString *)userID
                          orScreenName:(NSString *)screenName
                         optionalCount:(NSString *)count
                        optionalCursor:(NSString *)cursor
                          successBlock:(void(^)(NSArray *lists, NSString *previousCursor, NSString *nextCursor))successBlock
                            errorBlock:(void(^)(NSError *error))errorBlock;

/*
 POST	lists/members/destroy_all
 
 Removes multiple members from a list, by specifying a comma-separated list of member ids or screen names. The authenticated user must own the list to be able to remove members from it. Note that lists can't have more than 500 members, and you are limited to removing up to 100 members to a list at a time with this method.
 
 Please note that there can be issues with lists that rapidly remove and add memberships. Take care when using these methods such that you are not too rapidly switching between removals and adds on the same list.
 */

- (void)postListsMembersDestroyAllForListID:(NSString *)listID
                                    userIDs:(NSArray *)userIDs // array of strings
                              orScreenNames:(NSArray *)screenNames // array of strings
                               successBlock:(void(^)())successBlock
                                 errorBlock:(void(^)(NSError *error))errorBlock;

- (void)postListsMembersDestroyAllForSlug:(NSString *)slug
                          ownerScreenName:(NSString *)ownerScreenName
                                orOwnerID:(NSString *)ownerID
                                  userIDs:(NSArray *)userIDs // array of strings
                            orScreenNames:(NSArray *)screenNames // array of strings
                             successBlock:(void(^)())successBlock
                               errorBlock:(void(^)(NSError *error))errorBlock;

/*
 GET     lists/ownerships
 
 Returns the lists owned by the specified Twitter user. Private lists will only be shown if the authenticated user is also the owner of the lists.
 */

- (void)getListsOwnershipsForUserID:(NSString *)userID
                       orScreenName:(NSString *)screenName
                      optionalCount:(NSString *)count
                     optionalCursor:(NSString *)cursor
                       successBlock:(void(^)(NSArray *lists, NSString *previousCursor, NSString *nextCursor))successBlock
                         errorBlock:(void(^)(NSError *error))errorBlock;

#pragma mark Saved Searches

#pragma mark Places & Geo

//	GET		geo/id/:place_id

//	GET		geo/reverse_geocode
//	Returns Places (*: up to 20 places that match the lat/lon)
- (void)getGeoReverseGeocodeWithLatitude:(NSString *)latitude
                               longitude:(NSString *)longitude
                            successBlock:(void(^)(NSArray *places))successBlock
                              errorBlock:(void(^)(NSError *error))errorBlock;

//	GET		geo/search
//	Returns Places (*: places that match the lat/lon)
- (void)getGeoSearchWithLatitude:(NSString *)latitude
                       longitude:(NSString *)longitude
                    successBlock:(void(^)(NSArray *places))successBlock
                      errorBlock:(void(^)(NSError *error))errorBlock;

- (void)getGeoSearchWithIPAddress:(NSString *)ipAddress
                     successBlock:(void(^)(NSArray *places))successBlock
                       errorBlock:(void(^)(NSError *error))errorBlock;

- (void)getGeoSearchWithQuery:(NSString *)query
                 successBlock:(void(^)(NSArray *places))successBlock
                   errorBlock:(void(^)(NSError *error))errorBlock;

//	GET		geo/similar_places

//	POST	geo/place

#pragma mark Trends

#pragma mark Spam Reporting

- (void)postReportSpamWithScreenName:(NSString *)screenName
                            orUserID:(NSString *)userID
                        successBlock:(void(^)(id userProfile))successBlock
                          errorBlock:(void(^)(NSError *error))errorBlock;

#pragma mark OAuth

#pragma mark Help
//	GET		application/rate_limit_status
//	Returns ?
- (void)getRateLimitsForResources:(NSArray *)resources
					 successBlock:(void(^)(NSDictionary *rateLimits))successBlock
					   errorBlock:(void(^)(NSError *error))errorBlock;

@end
