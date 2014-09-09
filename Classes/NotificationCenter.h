
/****************************************************************************
  Copyright (c) 2013-2014 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#ifndef Foundation_NotificationCenter_h
#define Foundation_NotificationCenter_h

#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>
#include <list>
#include <memory>
#include <thread>
#include <mutex>

namespace Foundation{

struct NotificationObserver {
    std::function<void(void)> callback;
};

class NotificationCenter
{
    static std::shared_ptr<NotificationCenter> m_defaultCenter;
    std::unordered_map<std::string, std::list<NotificationObserver> > m_observers;
    mutable std::mutex m_mutex;
public:
    
    typedef std::unordered_map<std::string, std::list<NotificationObserver> >::const_iterator notification_const_iterator;
    typedef std::unordered_map<std::string, std::list<NotificationObserver> >::iterator       notification_iterator;
    typedef std::list<NotificationObserver>::const_iterator                                   observer_const_iterator;
    typedef std::list<NotificationObserver>::iterator                                         observer_iterator;
    
    /**
     * This method adds a function callback as an observer to a named method.
     * @param method the function callback.  Accepts void(void) methods or lambdas.
     * @param name the name of the notification you wish to observe.
     */
    observer_const_iterator addObserver(std::function<void(void)> method, const std::string& name);
    
    /**
     * This method adds a function callback as an observer to a given method.
     * @param method the function callback.  Accepts void(void) methods or lambdas.
     * @param name the name of the notification you wish to observe.
     */
    observer_const_iterator addObserver(std::function<void(void)> method, notification_iterator& notification);
    
    /**
     * This method removes an observer by iterator.
     * @param name the name of the notification you wish to remove a given observer from.
     * @param observer the iterator to the observer you wish to remove.
     */
    void removeObserver(const std::string& name, observer_const_iterator& observer);
    
    /**
     * This method removes an observer by iterator.
     * @param notification the iterator of the notification you wish to remove a given observer from.
     * @param observer the iterator to the observer you wish to remove.
     */
    void removeObserver(notification_iterator& notification, observer_const_iterator& observer);
    
    /**
     * This method removes all observers from a given notification, removing the notification 
     * from being tracked outright.
     * @param name the name of the notification you wish to remove.
     */
    void removeObservers(const std::string& name);
    
    /**
     * This method removes all observers from a given notification, removing the notification 
     * from being tracked outright.
     * @param notification the iterator of the notification you wish to remove.
     */
    void removeAllObservers(notification_iterator& notification);
    
    /**
     * This method posts a notification to a set of observers.
     * If successful, this function calls all callbacks associated with that notification and 
     * return true.  If no such notification exists, this function will print a warning to the 
     * console and return false.
     * @param name the name of the notification you wish to post.
     */
    bool postNotification(const std::string& name) const;
    
    /**
     * This method posts a notification to a set of observers.
     * If successful, this function calls all callbacks associated with that notification and 
     * return true.  If no such notification exists, this function will print a warning to the 
     * console and return false.
     * @param name the name of the notification you wish to post.
     */
    bool postNotification(notification_const_iterator& notification) const;
    
    /**
     * This method retrieves a notification iterator for a named notification.
     * The returned iterator may be used with the overloaded variants of postNotification, 
     * removeAllObservers, removeObserver, and addObserver to avoid string lookups.
     * @param name the name of the notification you wish to post.
     */
    notification_iterator getNotificationIterator(const std::string& notification);
    
    /**
     * This method returns the default global notification center.  You may alternatively 
     * create your own notification center without using the default notification center.
     */
    static std::shared_ptr<NotificationCenter> defaultCenter();
};

} // namespace Foundation

#endif // Foundation_NotificationCenter_h