/****************************************************************************
  Copyright (c) 2013-2014 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#include "NotificationCenter.h"
namespace Foundation{
std::shared_ptr<NotificationCenter> NotificationCenter::m_defaultCenter = nullptr;

NotificationCenter::observer_const_iterator NotificationCenter::addObserver(std::function<void()> method, const std::string& name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    NotificationObserver n;
    n.callback = method;
    m_observers[name].push_back(n);
    return --m_observers[name].end();
}

NotificationCenter::observer_const_iterator NotificationCenter::addObserver(std::function<void (void)> method, notification_iterator &notification)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    NotificationCenter::observer_const_iterator retVal = notification->second.end();
    if (notification != m_observers.end())
    {
        NotificationObserver n;
        n.callback = method;
        notification->second.push_back(n);
        retVal = --notification->second.end();
    }
    return retVal;
}

void NotificationCenter::removeObserver(const std::string& name, std::list<NotificationObserver>::const_iterator& observer)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    notification_iterator notification = m_observers.find(name);
    if (notification != m_observers.end())
    {
        notification->second.erase(observer);
    }
}

void NotificationCenter::removeObserver(notification_iterator& notification, std::list<NotificationObserver>::const_iterator& observer)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (notification != m_observers.end())
    {
        notification->second.erase(observer);
    }
}

void NotificationCenter::removeObservers(const std::string& name)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_observers.erase(name);
}

void NotificationCenter::removeAllObservers(notification_iterator& notification)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (notification != m_observers.end())
    {
        m_observers.erase(notification);
    }
}

bool NotificationCenter::postNotification(const std::string& name) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    notification_const_iterator i = m_observers.find(name);
    if (i != m_observers.end())
    {
        const std::list<NotificationObserver>& notiList = i->second;
        for (auto& ia : notiList)
        {
            ia.callback();
        }
        return true;
    }
    else
    {
        printf("WARNING: Notification \"%s\" does not exist.\n", name.data());
        return false;
    }
}

bool NotificationCenter::postNotification(NotificationCenter::notification_const_iterator& notification) const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (notification != m_observers.end())
    {
        const std::list<NotificationObserver>& notiList = notification->second;
        for (auto& i : notiList)
        {
            i.callback();
        }
        return true;
    }
    else
    {
        printf("WARNING: Notification \"%s\" does not exist.\n", notification->first.data());
        return false;
    }
}

NotificationCenter::notification_iterator NotificationCenter::getNotificationIterator(const std::string& notification)
{
    notification_iterator retVal;
    if (m_observers.find(notification) != m_observers.end())
    {
        retVal = m_observers.find(notification);
    }
    
    return retVal;
}

std::shared_ptr<NotificationCenter> NotificationCenter::defaultCenter()
{
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    if (!m_defaultCenter)
    {
        m_defaultCenter = std::shared_ptr<NotificationCenter>(new NotificationCenter);
    }
    
    return m_defaultCenter;
}

} // namespace Foundation