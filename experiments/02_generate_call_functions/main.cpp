#include <memory>
#include <string>
#include <iostream>
#include <functional>
#include <type_traits>

class Envelope
{
public:
	template <typename T>
	Envelope(T&& msg)
		: m_msg(std::make_shared<EnvelopedMessage<T>>(std::forward<T>(msg)))
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
};

struct Message1
{
	Message1(std::string msg)
		: m_message{ msg }
	{}

	std::string message() const
	{
		return m_message;
	}

	std::string m_message;
};

void handleMessage1(Message1& msg) {
	std::cout << "Msg1: " << msg.m_message << std::endl;
}

template<class T>
struct remove_cvref
{
    typedef std::remove_cv_t<std::remove_reference_t<T>> type;
};

template< class T >
using remove_cvref_t = typename remove_cvref<T>::type;

template<typename Function>
struct function_traits_helper;

template<typename R>
struct function_traits_helper<R(*)(void)>
{
    using result_type = R;
};

template<typename R, typename T1>
struct function_traits_helper<R(*)(T1)>
{
    using result_type = R;
    using argument_type = T1;
    using msg_type = remove_cvref_t<T1>;
};

template<class T>
struct function_traits :
    public function_traits_helper<typename std::add_pointer_t<remove_cvref_t<T>>>
{};

template <typename T>
auto generate(T f) {
    using f_details = function_traits<std::remove_pointer_t<decltype(f)>>;
    using f_arg = typename f_details::argument_type;
	using f_msg = typename f_details::msg_type;

	return [f](Envelope& msg){
		auto m = dynamic_cast<const Envelope::EnvelopedMessage<f_msg>*>(msg.m_msg.get())->m_payload;
		f(m);
	};
}

int main()
{
	/*
	 * In this example, we generate a lambda to convert enveloped messages back to their original messages.
	 */

	Message1 msg1{ "Hello" };
	std::cout << "Direct call to handler => ";
	handleMessage1(msg1);

	Envelope e1{std::move(msg1)};
	std::cout << "Direct call to envelope => ";
	std::cout << "e1: " << e1.message() << std::endl;

	auto fn = [](Envelope& msg){
		auto m = dynamic_cast<const Envelope::EnvelopedMessage<Message1>*>(msg.m_msg.get())->m_payload;
		handleMessage1(m);
	};
	std::cout << "Wrapped in a lambda => ";
	fn(e1);

	auto g = generate(handleMessage1);
	std::cout << "Generated => ";
	g(e1);

	return 0;
}
