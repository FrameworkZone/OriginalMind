/****************************************************************************
  Copyright (c) 2013-2014 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#ifndef Foundation_Notification_h
#define Foundation_Notification_h

#include <memory>
#include <string>
#include <typeinfo>

namespace Foundation{

class Notification
{
public:
    typedef std::shared_ptr<Notification> Ptr;

    Notification(){};

    /**
     * Returns the name of the notification.
     * The default implementation returns the class name.
     */
    virtual std::string name() const
    {
        return typeid(*this).name();
    }


protected:
    virtual ~Notification(){};
};

} // namespace Foundation

#endif // Foundation_Notification_h