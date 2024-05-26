#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>

class Envelope
{
public:
	template <typename T>
	Envelope(T&& msg)
		: m_msg(std::make_shared<EnvelopedMessage<T>>(std::forward<T>(msg)))
		, m_typeIndex(typeid(T))
	{}

	std::string message() const
	{
		return m_msg->message();
	}

	class MessageInterface
	{
	public:
		virtual ~MessageInterface() {}
		virtual std::string message() const = 0;
	};

	template<typename T>
	struct EnvelopedMessage : MessageInterface // note: breaks when I replace struct with class...
	{
	public:
		EnvelopedMessage(const T& msg)
			: m_payload(msg)
		{}

		std::string message() const override
		{
			return m_payload.message();
		}
	// private:
		T m_payload;
	};

	std::shared_ptr<const MessageInterface> m_msg;
	std::type_index m_typeIndex;
};

#endif