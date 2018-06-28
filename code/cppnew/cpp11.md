c++11新特性
1.变量和基本类型
	long long
	列表初始化
	nullptr常量
	constexpr 变量
	类型别名：typedef和using
	auto
	decltype
	类内初始化
2.字符次/向量/数组
	使用auto或decltype缩写类型
	范围for语句
	定义vector对象的vector
	vector列表初始化
	容器的cbegin和cend函数
	标准库begin和end函数
	使用auto和decltype简化声明
3.表达式
	除法的舍入规则：直接切除小数部分
	大括号列表赋值
	sizeof用于类成员
4.语句
	for
5.函数
	标准库initializer_list类
		可以处理不同数量实参的函数，前提是实参类型要相同；
	列表初始化返回值
	定义尾值返回类型
	使用decltype简化返回类型
	constexpr常量函数:（动态定义数组大小？） 
6.类
	=defult生成默认构造函数
	禁止使用拷贝函数=delete
	类对象成员的类内初始化
	委托构造函数：构造函数调用构造函数
	constexpr构造函数，编译时计算
	
7.IO库
	增加使用string为文件名
8.顺序容器
	array和forward_list
	容器的cbegin和cend函数
	容器的列表初始化
	容器的非成员函数swap
	容器insert成员的返回类型
	容器的emplace成员
	shrink_to_fit
	string的数值转换函数
9.泛型算法
	ambda表达式
		[capture list](parameter list) -> return type { function body }	
		lambda表达式中的尾置返回类型
		标准库bind函数
		auto newCallable = bind(callable, arg_list);
			auto fun2 = bind(&MyClass::fun2, p, _1);//有参数
			 auto fun1 = bind(&MyClass::fun1, my);

10.关联容器
	关联容器的列表初始化
	列表初始化pair的返回类型
	pair的列表初始化
	无序容器
11.动态内存
	智能指针
	std::auto_ptr
	shared_ptr类	
	动态分配对象的列表初始化
	auto和动态分配
	unique_ptr类
	weak_ptr类
	范围for语句不能应用于动态分配数组
	动态分配数组的列表初始化
	auto不能用于分配数组
	allocator::construct可使用任意构造函数
12.拷贝控制
	将=default用于拷贝控制类对对象
	使用=delete用于拷贝控制成员
	用移动类对象代替拷贝类对象
	右值引用
		左值：在赋值号左边，可以被赋值的值，可以取地址
		右值：在赋值号右边，取出值赋给其他变量的值
		左值引用：type & 引用名 = 左值表达式
		右值引用：type && 引用名 = 右值表达式
	标准库move函数
	移动构造函数和移动赋值	
	移动构造函数通常是noexcept
	移动迭代器
	引用限定成员函数
13.重载运算与类型转换
	function类模版
	explicit类型转换运算符
14.面向对象程序设计	
	虚函数的override和final指示符
	删除的拷贝控制和继承
	继承的构造函数：许派生类直接调用基类的构造函数
15.模版与泛型编程
	声明模版类型形参为友元
	模版类型别名
	模版函数的默认模版参数
	实例化的显示控制
	模版函数与尾置返回类型
	引用折叠规则
	用static_cast将左值转换为右值
	标准库forward函数
	完美转发
	可变参数模版与转发
16.多线程
	std::thread
	std::atomic
	std::condition_variable
	std::async
	std::future

17.元组
	tuple<const char*, int>tp = make_tuple(sendPack,nSendSize); //构造一个tuple


























