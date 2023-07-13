/**
 * @file ContextClient.h
 * @author Hulu
 * @brief 客户端Context类定义
 * @version 0.1
 * @date 2023-07-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once 

#include "Context.h"
#include "coap/Information/GeneralInformation.h"

#include <map>

namespace CoapPlusPlus
{

class Session;
class ContextClient : public Context
{
public:
    /**
     * @brief 构造一个管理客户端相关信息的Context对象
     * @details coap_context_t的C++封装，一个Context对应一个服务器或者一个客户端。
     *          CoAP协议栈的全局状态被存储在coap_context_t对象中。 
     * 
     * @exception InternalException 创建Context失败会抛出该异常
     */
    ContextClient();
    ~ContextClient() noexcept;

    /**
     * @brief 为客户端Context添加一个会话，用于客户端与服务器之间的一个单独的交互。
     * 
     * @param port 服务器端口号
     * @param pro 使用的协议，默认为UDP
     * 
     * @return 是否添加成功
     *      @retval false 已经存在该会话或者内部错误
     *      @retval true 添加成功
     * 
     * @note 目前仅支持本地连接，即服务器地址为localhost
     */
    bool addSession(uint16_t port, Information::Protocol pro = Information::Udp) noexcept;

    /**
     * @brief 为客户端Context移除一个会话。
     * 
     * @param port 要移除的会话使用的端口号
     * @param pro 要移除的会话使用的协议
     * 
     * @return 是否移除成功，如果不存在该会话则移除失败
     */
    bool removeSession(uint16_t port, Information::Protocol pro) noexcept;

    /**
     * @brief 得到一个会话对象
     * 
     * @param port 会话使用的端口号
     * @param pro 会话使用的协议
     * @return 会话对象的引用
     * 
     * @exception TargetNotFoundException 未找到对应的会话会抛出该异常
     */
    Session& getSession(uint16_t port, Information::Protocol pro) const;

    /**
     * @brief 得到当前Context中的所有会话数量
     * 
     * @return 会话数量
     */
    size_t getSessionCount() const noexcept { return m_sessions.size(); }

private:
    bool isReady() const noexcept override;

    /**
     * @brief 创建一个会话对象
     * 
     * @return 会话对象的指针
     * 
     * @exception InternalException 创建会话失败会抛出该异常
     */
    coap_session_t* createSession(uint16_t port, Information::Protocol pro);

private:
    using SessionKey = std::pair<uint16_t, Information::Protocol>;
    std::map<SessionKey, Session*> m_sessions;
};


};// namespace CoapPlusPlus