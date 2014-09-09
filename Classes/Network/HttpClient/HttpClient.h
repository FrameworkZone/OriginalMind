
#ifndef Network_HttpClient_h
#define Network_HttpClient_h

#include "HttpRequest.h"
#include "HttpResponse.h"
#include "HttpClient.h"

namespace Network {

/**
 * @addtogroup Network
 * @{
 */


/** @brief Singleton that handles asynchrounous http requests
 * Once the request completed, a callback will issued in main thread when it provided during make request
 */
class HttpClient
{
public:
    /** Return the shared instance **/
    static HttpClient *getInstance();
    
    /** Relase the shared instance **/
    static void destroyInstance();

    /** Enable cookie support. **/
    void enableCookies(const char* cookieFile);
        
    /**
     * Add a get request to task queue
     * Please make sure request->_requestData is clear before calling "sendAsynchronousRequest" here.
     * @param request a HttpRequest object, which includes url, response callback etc.
     */
    bool sendAsynchronousRequest(HttpRequest::pointer request);

    std::string sendSynchronousRequest(HttpRequest::pointer request, int& error);
  
    
    /**
     * Change the connect timeout
     * @param value The desired timeout.
     */
    inline void setTimeoutForConnect(int value) {_timeoutForConnect = value;};
    
    /**
     * Get connect timeout
     * @return int
     */
    inline int getTimeoutForConnect() {return _timeoutForConnect;}
    
    
    /**
     * Change the download timeout
     * @param value
     */
    inline void setTimeoutForRead(int value) {_timeoutForRead = value;};
    

    /**
     * Get download timeout
     * @return int
     */
    inline int getTimeoutForRead() {return _timeoutForRead;};
        
private:
    HttpClient();
    virtual ~HttpClient();
    bool init(void);
    
    /**
     * Init pthread mutex, semaphore, and create new thread for http requests
     * @return bool
     */
    bool lazyInitThreadSemphore();
    void networkThread();
    /** Poll function called from main thread to dispatch callbacks when http requests finished **/
    void dispatchResponseCallbacks();
    
private:
    int _timeoutForConnect;
    int _timeoutForRead;
};

}
#endif // Network_HttpClient_h

