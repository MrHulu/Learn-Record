#pragma once

#include <coap3/coap.h>
#include "coap/ResourceInterface.h"
#include "coap/Session.h"
#include "coap/DataStruct/Address.h"
#include "coap/Pdu/RequestPdu.h"
#include "coap/Pdu/ResponsePdu.h"
#include "coap/Pdu/Option.h"

namespace CoapPlusPlus
{

class TestResourceInterfaceData
{
    friend class TestResourceInterface;
public:
    TestResourceInterfaceData(int number) noexcept : m_number(number) {  }
    bool operator==(const TestResourceInterfaceData& other) const noexcept { return m_number == other.m_number; }
    int number() const noexcept { return m_number; }

private:
    int m_number = 0;
};

class TestResourceInterface : public ResourceInterface {

public:
    TestResourceInterface(TestResourceInterfaceData* data, Information::RequestCode code) noexcept 
        : m_data(data)
        , ResourceInterface(code) {  }

    ~TestResourceInterface() noexcept { }

    void onRequest(Session session, std::string query, ResponsePdu response, RequestPdu request) override
    {
        auto code = request.code();
        switch (code)
        {
        case Information::RequestCode::Get:
            m_data->m_number++;
            break;
        case Information::RequestCode::Post:
            m_data->m_number--;
            break;
        case Information::RequestCode::Delete:
            m_data->m_number = m_data->m_number * (-1);
            break;
        case Information::RequestCode::Put:
            m_data->m_number = m_data->m_number * m_data->m_number;
            break;
        }
        auto testlog = [](Pdu* pdu) {
            Log::Logging(INFO, "Mid[%d] Type[%d]\n", pdu->messageId(), pdu->messageType());
            Log::Logging(INFO, "Option[ ");
            for(auto opt: pdu->getOptions())
                Log::Logging(INFO, " %d ", opt.getNumber());
            Log::Logging(INFO, " ]\n");
            auto vec = pdu->payload().data();
            Log::Logging(INFO, "Payload[ %s ]\n", std::string(vec.begin(), vec.end()).c_str());
            Log::Logging(INFO, "*********************\n");
        };
        Log::Logging(INFO, "\n******** request  ******\n");
        Log::Logging(INFO, "Token[%s]\n", request.token().toHexString().c_str());
        Log::Logging(INFO, "Code[%d]\n", request.code());
        testlog(&request);

        Log::Logging(INFO, "\n******** response  ******\n");
        Log::Logging(INFO, "Code[%d]\n", response.code());
        testlog(&response);
        
        Log::Logging(INFO, "LocalPort:%d, RemotePort:%d\n", session.getLocalAddress().getPort(), session.getRemoteAddress().getPort());
        Log::Logging(INFO, "Request:"); Pdu::LogPdu(INFO, &request);
        Log::Logging(INFO, "Response:"); Pdu::LogPdu(INFO, &response);
    }

    TestResourceInterfaceData* data() noexcept { return m_data; }

private:
    TestResourceInterfaceData* m_data = nullptr;
};


};