c++17新特性

UTF-8字符文字
折叠式表达式：用于可变模板
内联变量：头文件中定义变量
在if和switch语句内可以初始化变量
结构化绑定：for(auto[key,value]:my_map){};
模板参数规约

static——assert的文本信息可选
删除trigraphs
在模板参数中允许使用 typename
来自 braced-init-list 的新规则用于自动推导
嵌套命名空间的定义
允许命名空间和枚举器的属性
新的标准属性：[[fallthrough]], [[maybe_unused]] 和 [[nodiscard]]；
对所有非类型模板参数进行常量评估
Fold 表达式，用于可变的模板
一些用于对齐内存分配的扩展；
构造函数的模板推导，允许使用 std::pair(5.0, false) 代替 std::pair<double,bool>(5.0, false)；
__has_include，允许由预处理程序指令检查头文件的可用性；
std::string_view 和 std::optional 
boost::filesystem
