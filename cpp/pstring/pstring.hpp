// 2018-01-07
#ifndef __HEADER__PSTRING_
#define __HEADER__PSTRING_
#include<stdexcept>
#include<iostream>
class pstring {
private:
    // 任意迭代器类型
    template<typename FN>
    struct any_iterator {
    private:
        char *p;
    public:
        any_iterator(): p(nullptr) {}
        any_iterator(char *p): p(p) {}

        char &operator*()
        {
            return *p;
        }

        char *operator->()
        {
            return p;
        }

        bool operator!=(const any_iterator& rhs) const
        {
            return p != rhs.p;
        }

        any_iterator &operator++()
        {
            FN f;
            p = f(p);
            return *this;
        }
    };

    // 任意常量迭代器类型
    template<typename FN>
    struct any_const_iterator {
    private:
        char *p;
    public:
        any_const_iterator(): p(nullptr) {}
        any_const_iterator(char *p): p(p) {}

        const char &operator*()
        {
            return *p;
        }

        const char *operator->()
        {
            return p;
        }

        bool operator!=(const any_const_iterator& rhs) const
        {
            return p != rhs.p;
        }

        any_const_iterator &operator++()
        {
            FN f;
            p = f(p);
            return *this;
        }
    };

    // 正向迭代器函数
    struct FNIterator {
        char *operator()(char *p)
        {
            return ++p;
        }
    };

    // 逆向迭代器函数
    struct FNReverseIterator {
        char *operator()(char *p)
        {
            return --p;
        }
    };

public:
    /** @type size_type 字符串中的所有长度、大小类型 */
    typedef unsigned int size_type;
    /** @type iterator 字符串内容迭代器 */
    typedef any_iterator<FNIterator> iterator;
    typedef any_const_iterator<FNIterator> const_iterator;
    /** @type reverse_iterator 字符串内容逆向迭代器 */
    typedef any_iterator<FNReverseIterator> reverse_iterator;
    typedef any_const_iterator<FNReverseIterator> const_reverse_iterator;
private:    
    // 存储默认长度
    static const size_type DEFAULT_CAPACITY = 10;
    // 常数，表示最长支持的字符串长度（此处用到隐式转换）
    static const size_type npos = -1;
    // 存储区域的指针
    char *_data = nullptr;
    // 目前字符的长度
    size_type _length;
    // 目前存储区域的大小（容量）
    size_type _capacity = DEFAULT_CAPACITY;
    /** 求C风格字符串长度
     *  @param st {const char *} 要求长的C风格字符串
     *  @return {size_type} 字符串长度
     */
    size_type count_cstring(const char *st) const;
public:
    /** @constructor 构建一个空字符串 */
    pstring();

    /** @destructor 回收字符串资源 */
    ~pstring();

    /** @constructor 构造由指定C风格字符串构成的字符串
     *  @param st {const char *} 指定C风格字符串
     */
    pstring(const char *s);

    /** @constructor 构造由指定字符串构成的字符串
     *  @param str {const pstring &} 指定字符串
     */
    pstring(const pstring &st);

    /** @constructor 构造由count个指定字符构成的字符串
     *  @param ch {const char} 指定字符
     *  @param count {int} 个数
     */
    pstring(const char ch, int count = 1);

    /** 用另一个字符串赋值
     *  @param st {const pstring} 另一个字符串
     *  @return {pstring &} *this
     */
    pstring &assign(const pstring &st);

    /** 用一个C风格字符串赋值
     *  @param s {const char *} C风格字符串
     *  @return {pstring &} *this
     */
    pstring &assign(const char *s);

    /** 用一个字符赋值
     *  @param ch {const char} 指定字符
     *  @param count {int} 字符出现的次数
     *  @return {pstring &} *this
     */
    pstring &assign(const char ch, int count = 1);

    /** 用另一个字符串赋值，等同于assign(const pstring &)
     *  @see assign(const pstring &st)
     */
    pstring &operator=(const pstring &st);

    /** 用一个C风格字符串赋值，等同于assign(const char *)
     *  @see assign(const char *s)
     */
    pstring &operator=(const char *s);

    /** 用一个字符赋值，等同于assign(const char ch)
     *  @see assign(const char ch)
     */
    pstring &operator=(const char ch);

    /** 返回位于idx的字符的引用。并进行边界检查，非法访问时抛出 std::out_of_range 类型的异常。
     *  @param idx 指定的位置
     *  @throws std::out_of_range
     *  @return {char &} 指定位置字符的引用
     */
    char &at(int idx);

    /** 返回位于idx的字符的引用。但不进行边界检查。
     *  @param idx 指定的位置
     *  @return {char &} 指定位置字符的引用
     */
    char &operator[](int idx) noexcept;

    /** 返回位于idx的字符的引用。但不进行边界检查。
     *  @param idx 指定的位置
     *  @return {char &} 指定位置字符的引用
     */
    const char &operator[](int idx) const;

    /** 返回string首字符的引用。当字符串为空时将出现随机字符。
     *  @return {char &} 首字符的引用
     */
    char &front();

    /** 返回string首字符的引用。当字符串为空时将出现随机字符。
     *  @return {const char &} 首字符的引用
     */
    const char &front() const;

    /** 返回string尾字符的引用。当字符串为空时将出现随机字符。
     *  @return {char &} 尾字符的引用
     */
    char &back();

    /** 返回string尾字符的引用。当字符串为空时将出现随机字符。
     *  @return {const char &} 尾字符的引用
     */
    const char& back() const;

    /** 获得字符串存储位置的指针。
     *  @return {char *} 数据指针
     */
    char *data();

    /** 获得字符串存储位置的指针 
     *  @return {const char *} 数据指针
     */
    const char *data() const;

    /** 获得string对应的C风格字符串 
     *  @return {const char *} 对应的C风格的字符串
     */
    const char *c_str() const;

    /** 返回从首字符开始的正向迭代器
     *  @return {iterator} 从首字符开始的正向迭代器
     */
    iterator begin() const;

    /** 返回从首字符开始的常量正向迭代器
     *  @return {iterator} 从首字符开始的常量正向迭代器
     */
    const_iterator cbegin() const;

    /** 返回从首字符开始的逆向迭代器
     *  @return {iterator} 从首字符开始的逆向迭代器
     */
    reverse_iterator rbegin() const;

    /** 返回从首字符开始的常量逆向迭代器
     *  @return {iterator} 从首字符开始的常量逆向迭代器
     */
    const_reverse_iterator crbegin() const;


    /** 返回从尾字符开始的正向迭代器
     *  @return {iterator} 从尾字符开始的正向迭代器
     */
    iterator end() const;

    /** 返回从尾字符开始的常量正向迭代器
     *  @return {iterator} 从尾字符开始的常量正向迭代器
     */
    const_iterator cend() const;

    /** 返回从尾字符开始的逆向迭代器
     *  @return {iterator} 从尾字符开始的逆向迭代器
     */
    reverse_iterator rend() const;

    /** 返回从尾字符开始的常量逆向迭代器
     * @return {iterator} 从尾字符开始的常量逆向迭代器
     */
    const_reverse_iterator crend() const;

    /** 获得字符串是否为空字符串 
     *  @return {bool} 字符串是否为空
     */
    bool empty() const;

    /** 获得字符串的长度，等价于length()
     *  @return {size_type} 字符串的长度
     */
    size_type size() const;

    /** 获得字符串的长度，等价于size()
     *  @return {size_type} 字符串的长度
     */
    size_type length() const;

    /** 获得字符串库允许的最大长度
     *  @return {size_type} 库允许的最大字符串的长度
     */
    size_type max_size() const;

    /** 指定string预留一定的空间，具体行为以参数的不同而不同。
     *  若new_cap大于当前的capacity(), 则分配新存储，并使capacity()大于或等于new_cap；
     *  若new_cap小于当前的capacity()，则不会发生任何事；
     *  若new_cap小于当前的size()，则等同于执行了shrink_to_fit()。
     *  @param new_cap 新的空间的参考值
     *  @return {void}
     */
    void reserve(size_type new_cap = 0);

    /** 返回已经为string分配空间的字符数。
     *  @return {size_type} 表示已分配的字符数
     */
    size_type capacity() const;

    /** 请求移除未使用的容量，减少capacity()到size()的请求。
     *  一旦执行，将非法化所有指针、引用和迭代器。
     *  @return {void}
     */
    void shrink_to_fit();

    /** 擦除string中的所有字符，同时所有指针、引用、迭代器均非法化。
     *  @return {void}
     */
    void clear();
    
    /** 在index处插入count个ch字符。
     *  @param index {size_type} 在何处插入
     *  @param count {size_type} 插入字符数量
     *  @param ch {char} 要插入的字符
     *  @return {pstring &} *this
     */
    pstring &insert(size_type index, size_type count, char ch);

    /** 在index处插入s所指向的C风格字符串。
     *  @param index {size_type} 在何处插入
     *  @param s {const char *} 要插入的C风格的字符串
     *  @return {pstring &} *this
     */
    pstring &insert(size_type index, const char *s);

    /** 在index处插入s所指向字符串的首count个字符。
     *  @param index {size_type} 在何处插入
     *  @param s {const char *} 要插入的C风格的字符串
     *  @param count {size_type} 要插入前count个字符
     *  @return {pstring &} *this
     */
    pstring &insert(size_type index, const char *s, size_type count);

    /** 在index处插入字符串str 。
     *  @param index {size_type} 在何处插入
     *  @param str {const pstring &} 要插入的字符串
     *  @return {pstring &} *this
     */
    pstring &insert(size_type index, const pstring &str);

    /** 移除从index开始的min(count, size() - index)个字符。
     *  @param index {size_type} 从何处开始移除
     *  @param count {size_type} 移除多少个字符
     *  @return {pstring &} *this
     */
    pstring &erase(size_type index, size_type count = npos);

    /** 将ch插入到字符串的末尾
     *  @param ch {char} 要插入的字符
     *  @return {void}
     */
    void push_back(char ch);

    /** 删除字符串末尾的一个字符，若字符串为空，将什么也不做。
     *  @return {void}
     */
    void pop_back();

    /** 在字符串后附加count个ch字符。
     *  @param count {size_type} 要附加多少个字符
     *  @param ch {char} 要附加的字符
     *  @return {pstring &} *this
     */
    pstring &append(char ch, size_type count = 1);
    
    /** 在字符串后附加str字符串
     *  @param str {const pstring &} 要附加的字符串
     *  @return {pstring &} *this
     */
    pstring &append(const pstring &str);

    /** 在字符串后附加s的前count个字符
     *  @param s {const char *} 要附加的C风格字符串
     *  @param count {size_type} 前count个字符
     *  @return {pstring &} *this
     */
    pstring &append(const char *s, size_type count);

    /** 在字符串后附加s所指向的一个C风格字符串
     *  @param s {const char *} 要附加的C风格字符串
     *  @return {pstring &} *this
     */
    pstring &append(const char *s);

    /** 在字符串后附加str字符串，等同于append(str)
     *  @see append(const pstring &str)
     */
    pstring &operator +=(const pstring &str);

    /** 在字符串后附加ch字符，等同于append(ch)
     *  @see append(char ch)
     */
    pstring &operator +=(char ch);

    /** 在字符串后附加s所指向的一个C风格字符串，等同于append(s)
     *  @see append(const char *s)
     */
    pstring &operator +=(const char *s);

    /** 比较两字符串
     *  @param str {const pstring &} 要比较的字符串
     *  @return {int} 当两字符串相等时，返回0；
     *                str串比本串大时，返回负数；
     *                str串比本串小时，返回正数。
     */
    int compare(const pstring &str) const;

    /** 比较两字符串
     *  @param str {const char *} 要比较的字符串
     *  @return {int} 当两字符串相等时，返回0；
     *                str串比本串大时，返回负数；
     *                str串比本串小时，返回正数。
     */
    int compare(const char *str) const;

    /** 判断字符串是否以指定字符串为前缀
     *  @param str {pstring &} 指定的字符串
     *  @return {bool} 字符串是否以之为前缀
     */
    bool starts_with(pstring &str) const noexcept;

    /** 判断字符串是否以指定字符为前缀
     *  @param str {char} 指定的字符
     *  @return {bool} 字符串是否以之为前缀
     */
    bool starts_with(char x) const noexcept;

    /** 判断字符串是否以指定C风格字符串为前缀
     *  @param str {const char *} 指定的C风格字符串
     *  @return {bool} 字符串是否以之为前缀
     */
    bool starts_with(const char *x);

    /** 判断字符串是否以指定字符串为后缀
     *  @param str {pstring &} 指定的字符串
     *  @return {bool} 字符串是否以之为后缀
     */
    bool ends_with(pstring &str) const noexcept;

    /** 判断字符串是否以指定字符为后缀
     *  @param str {char} 指定的字符
     *  @return {bool} 字符串是否以之为后缀
     */
    bool ends_with(char x) const noexcept;

    /** 判断字符串是否以指定C风格字符串为后缀
     *  @param str {const char *} 指定的C风格字符串
     *  @return {bool} 字符串是否以之为后缀
     */
    bool ends_with(const char *x);


    /** 以指定的字符串替换原字符串中[pos, pos + count)部分。
     *  @param pos {size_type} 从何处开始
     *  @param count {size_type} 替换多少个字符
     *  @param str {const pstring &} 指定的字符串
     *  @return {const pstring &} *this
     */
    pstring &replace(size_type pos, size_type count, const pstring &str);

    /** 以指定的字符串替换原字符串中[pos, pos + count)部分。
     *  @param pos {size_type} 从何处开始
     *  @param count {size_type} 替换多少个字符
     *  @param cstr {const char *} 指定的C风格字符串
     *  @return {const pstring &} *this
     */
    pstring &replace(size_type pos, size_type count, const char *cstr);

    /** 以指定的字符替换原字符串中pos位置的字符。
     *  @param pos {size_type} 指定位置
     *  @param ch {char} 指定的字符
     *  @return {const pstring &} *this
     */
    pstring &replace(size_type pos, char ch);

    /** 获得字符串的子串，相当于字符串[pos, pos + count)部分。
     *  若请求的字符串越过字符串的结尾，或count == npos，则返回的子串为从pos位置到结束。
     *  @param pos {size_type} 指定位置
     *  @param count {size_type} 字符数量
     *  @return {pstring} 该字符串的一个子串
     */
    pstring substr(size_type pos = 0, size_type count = npos) const;


    /** 将字符串的子串拷贝到一个C风格字符串中，拷贝的字符串为[pos, pos + count)部分。
     *  若请求的字符串越过字符串的结尾，或count == npos，则返回的子串为从pos位置到结束。
     *  若pos > size(), 则抛出 std::out_of_range
     *  @param dest {char *} 
     *  @param count {size_type} 字符数量
     *  @param pos {size_type} 指定位置
     *  @throws std::out_of_range
     *  @return {size_type} 总共复制的字符数
     */
    size_type copy(char *dest, size_type count, size_type pos = 0) const;


    /** 重设当前字符串大小
     *  若当前大小小于count, 则将以指定的字符填充。
     *  若当前大小大于count, 则将缩减字符串到count位置
     *  ！！特别说明：当ch='\0'时，将不会用填充字符填充！！
     *  @param count {size_type} 新的字符串大小
     *  @param ch {char} 填充字符
     *  @return {void}
     */
    void resize(size_type count, char ch = '\0');
    
    /** 交换string和other的内容，并非法化所有迭代器和引用
     *  @param other {pstring &} 需要交换的字符串
     *  @return {void}
     */
    void swap(pstring &other) noexcept;

    /** 在字符串中查找其他字符串
     *  @param st {const char *} 要查找的串
     *  @param from {size_type} 从何处开始查找
     *  @return {size_type} 串的位置，不存在则返回npos
     */
    size_type find(const char *st, size_type from = 0);

    /** 在字符串中查找其他字符串
     *  @param st {const pstring &} 要查找的串
     *  @param from {size_type} 从何处开始查找
     *  @return {size_type} 串的位置，不存在则返回npos
     */
    size_type find(const pstring &st, size_type from = 0);

    /** 在字符串中查找其他字符
     *  @param st {const char} 要查找的字符
     *  @param from {size_type} 从何处开始查找
     *  @return {size_type} 字符的位置，不存在则返回npos
     */
    size_type find(const char ch, size_type from = 0);

    /** 生成两个字符串前后连接的一个新字符串
     *  @param lhs {const pstring &lhs} 位于新串前方的串
     *  @param rhs {const pstring &lhs} 位于新串后方的串
     *  @return {pstring} 产生的新字符串
     */
    pstring operator+(const pstring &rhs);

    /** 生成一个字符串和一个C风格字符串前后连接的一个新字符串
     *  @param lhs {const pstring &lhs} 位于新串前方的串
     *  @param rhs {const pstring &lhs} 位于新串后方的C风格字符串
     *  @return {pstring} 产生的新字符串
     */
    pstring operator+(const char *rhs);

    /** 生成一个字符串和一个字符前后连接的一个新字符串
     *  @param lhs {const pstring &lhs} 位于新串前方的串
     *  @param rhs {const pstring &lhs} 位于新串后方的字符
     *  @return {pstring} 产生的新字符串
     */
    pstring operator+(const char rhch);

    /** 比较本串与other串，返回本串是否等于other串
     *  @param other {const pstring &} 要比较的串
     *  @return {bool} 是否相等
     */
    bool operator==(const pstring &other) const;
    bool operator==(const char *other) const;
    friend bool operator==(const char *cstr, const pstring &str);

    /** 比较本串与other串，返回本串是否不等于other串
     *  @param other {const pstring &} 要比较的串
     *  @return {bool} 是否不等
     */
    bool operator!=(const pstring &other) const;
    bool operator!=(const char *other) const;
    friend bool operator!=(const char *cstr, const pstring &str);

    /** 比较本串与other串，返回本串是否小于other串
     *  @param other {const pstring &} 要比较的串
     *  @return {bool} 是否小于other串
     */
    bool operator<(const pstring &other) const;
    bool operator<(const char *other) const;
    friend bool operator<(const char *cstr, const pstring &str);

    /** 比较本串与other串，返回本串是否大于other串
     *  @param other {const pstring &} 要比较的串
     *  @return {bool} 是否大于other串
     */
    bool operator>(const pstring &other) const;
    bool operator>(const char *other) const;
    friend bool operator>(const char *cstr, const pstring &str);

    /** 比较本串与other串，返回本串是否小于等于other串
     *  @param other {const pstring &} 要比较的串
     *  @return {bool} 是否小于等于other串
     */
    bool operator<=(const pstring &other) const;
    bool operator<=(const char *other) const;
    friend bool operator<=(const char *cstr, const pstring &str);

    /** 比较本串与other串，返回本串是否大于等于other串
     *  @param other {const pstring &} 要比较的串
     *  @return {bool} 是否大于等于other串
     */
    bool operator>=(const pstring &other) const;
    bool operator>=(const char *other) const;
    friend bool operator>=(const char *cstr, const pstring &str);

    friend std::ostream &operator<<(std::ostream &out, const pstring &str);
    friend std::istream &operator>>(std::istream &in, const pstring &str);
};

pstring::size_type pstring::count_cstring(const char *st) const
{
    size_type r = 0;
    while (*st != '\0') {
        st++, r++;
    }

    return r;
}

pstring::pstring()
{
    _length = 0;
    _data = new char[DEFAULT_CAPACITY];
}

pstring::~pstring()
{
    delete[] _data;
}

pstring::pstring(const char *s)
{
    assign(s);
}

pstring::pstring(const pstring &st)
{
    assign(st);
}

pstring::pstring(const char ch, int count)
{
    assign(ch, count);
}

pstring &pstring::assign(const pstring &st)
{
    if (this == &st)
        return *this;

    if (_data != nullptr)
        delete[] _data;

    _length = st._length;
    _capacity = st._capacity;
    _data = new char[_capacity];

    for (int i = 0; i < _length; i++) {
        _data[i] = st._data[i];
    }
    _data[_length] = '\0';
    return *this;
}

pstring &pstring::assign(const char *s)
{
    if (_data != nullptr)
        delete[] _data;

    _length = count_cstring(s);
    _capacity = _length + DEFAULT_CAPACITY;
    _data = new char[_capacity];

    for (int i = 0; i < _length; i++) {
        _data[i] = s[i];
    }
    _data[_length] = '\0';
    return *this;
}

pstring &pstring::assign(const char ch, int count)
{
    if (_data != nullptr)
        delete[] _data;

    _length = count;
    _capacity = _length + DEFAULT_CAPACITY;
    _data = new char[_capacity];

    for (int i = 0; i < _length; i++) {
        _data[i] = ch;
    }
    _data[_length] = '\0';
    return *this;
}

pstring &pstring::operator=(const pstring &st)
{
    return assign(st);
}

pstring &pstring::operator=(const char *s)
{
    return assign(s);
}

pstring &pstring::operator=(const char ch)
{
    return assign(ch);
}

char &pstring::at(int idx)
{
    if (idx < _length)
        return _data[idx];
    throw std::out_of_range("Index Overflow");
}

char &pstring::operator[](int idx) noexcept
{
    return _data[idx];
}

const char &pstring::operator[](int idx) const
{
    return _data[idx];
}

char &pstring::front()
{
    return _data[0];
}

const char &pstring::front() const
{
    return _data[0];
}

char &pstring::back()
{
    return _data[_length - 1];
}

const char &pstring::back() const 
{
    return _data[_length - 1];
}

char *pstring::data()
{
    return _data;
}

const char *pstring::data() const
{
    return _data;
}

const char *pstring::c_str() const
{
    return _data;
}

pstring::iterator pstring::begin() const
{
    return iterator(_data);
}

pstring::const_iterator pstring::cbegin() const
{
    return const_iterator(_data);
}

pstring::reverse_iterator pstring::rbegin() const
{
    return reverse_iterator(_data + _length - 1);
}

pstring::const_reverse_iterator pstring::crbegin() const
{
    return const_reverse_iterator(_data + _length - 1);
}

pstring::iterator pstring::end() const 
{
    return iterator(_data + _length);
}

pstring::const_iterator pstring::cend() const
{
    return const_iterator(_data + _length);
}

pstring::reverse_iterator pstring::rend() const
{
    return reverse_iterator(_data - 1);
}

pstring::const_reverse_iterator pstring::crend() const
{
    return const_reverse_iterator(_data - 1);
}

bool pstring::empty() const
{
    return _length == 0;
}

pstring::size_type pstring::size() const
{
    return _length;
}

pstring::size_type pstring::length() const
{
    return _length;
}

pstring::size_type pstring::max_size() const
{
    return npos;
}

void pstring::reserve(pstring::size_type new_cap) 
{
    if (new_cap > capacity()) {
        _capacity = new_cap + DEFAULT_CAPACITY;
        char *store = new char[_capacity];
        for (int i = 0; i < _length; i++) {
            store[i] = _data[i];
        }
        store[_length] = '\0';
        delete[] _data;
        _data = store;
    } else if (new_cap < size()) {
        shrink_to_fit();
    }
}

pstring::size_type pstring::capacity() const
{
    return _capacity;
}

void pstring::shrink_to_fit()
{
    if (_capacity - _length < DEFAULT_CAPACITY)
        return;
    _capacity = DEFAULT_CAPACITY + _length;
    char *store = new char[_capacity];
    for (int i = 0; i < _length; i++) {
        store[i] = _data[i];
    }
    store[_length] = '\0';
    delete[] _data;
    _data = store;
}

void pstring::clear()
{
    _data[0] = '\0';
    _length = 0;
}

pstring &pstring::insert(pstring::size_type index, pstring::size_type count, char ch)
{
    index = (index < _length) ? index : _length;
    int idx = index;
    if (_length + count >= capacity())
        reserve(_length);

    for (int i = _length; i >= idx; i--) {
        _data[i + count] = _data[i];
    }

    for (int i = 0; i < count; i++) {
        _data[index + i] = ch;
    }
    _length += count;
    return *this;
}

pstring &pstring::insert(pstring::size_type index, const char *s)
{
    int len = count_cstring(s);
    return insert(index, s, len);
}

pstring &pstring::insert(pstring::size_type index, const char *s, pstring::size_type count)
{
    index = (index < _length) ? index : _length;
    int idx = index;
    if (_length + count >= capacity())
        reserve(_length);

    for (int i = _length; i >= idx; i--) {
        _data[i + count] = _data[i];
    }

    for (int i = 0; i < count; i++) {
        _data[index + i] = s[i];
    }
    _length += count;
    _data[_length] = '\0';

    return *this;
}

pstring &pstring::insert(pstring::size_type index, const pstring &str)
{
    return insert(index, str.data());
}

pstring &pstring::erase(pstring::size_type index, pstring::size_type count)
{
    for (int i = index; i <= _length - count; i++) {
        _data[i] = _data[i + count];
    }

    _length -= count;
    _data[_length] = '\0';

    return *this;
}

void pstring::push_back(char ch)
{
    _data[_length++] = ch;
    _data[_length] = '\0';
}

void pstring::pop_back()
{
    if (_length > 0)
        _data[--_length] = '\0';
}

pstring &pstring::append(char ch, pstring::size_type count)
{
    for (int i = 0; i < count; i++) {
        _data[_length + i] = ch;
    }
    _length += count;
    _data[_length] = '\0';

    return *this;
}

pstring &pstring::append(const pstring &str)
{
    return append(str.data());
}

pstring &pstring::append(const char *s, pstring::size_type count)
{
    if (_length + count >= capacity())
        reserve(_length + count);

    for (int i = 0; i < count; i++) {
        _data[_length + i] = s[i];
    }
    _length += count;
    _data[_length] = '\0';
    return *this;
}

pstring &pstring::append(const char *s)
{
    int len = count_cstring(s);
    return append(s, len);
}

pstring &pstring::operator +=(const pstring &str)
{
    return append(str);
}

pstring &pstring::operator +=(const char *s)
{
    return append(s);
}

pstring &pstring::operator +=(char ch)
{
    return append(ch);
}

int pstring::compare(const pstring &str) const 
{
    return compare(str.data());
}

int pstring::compare(const char *str) const
{
    auto strcmp = [](const char *a, const char *b) -> int {
        while ((*a) && (*a == *b)) {
            a++;
            b++;
        }

        if (*(unsigned char*)a > *(unsigned char *)b)
            return 1;
        else if (*(unsigned char*)a < *(unsigned char *)b)
            return -1;
        return 0;
    };
    return strcmp(data(), str);
}

bool pstring::starts_with(pstring &str) const noexcept
{
    if (str.length() > length())
        return false;
    for (int i = 0; i < str.length(); i++) {
        if (_data[i] != str[i])
            return false;
    }
    return true;
}

bool pstring::starts_with(const char *x)
{
    int len = count_cstring(x);
    if (len > length())
        return false;

    for (int i = 0; i < len; i++) {
        if (_data[i] != x[i])
            return false;
    }
    return true;
}

bool pstring::starts_with(char x) const noexcept
{
    return _data[0] == x;
}

bool pstring::ends_with(pstring &str) const noexcept
{
    if (str.length() > length() || empty())
        return false;
    int starts_from = length() - str.length();
    for (int i = 0; i < str.length(); i++) {
        if (_data[starts_from + i] != str[i])
            return false;
    }
    return true;
}

bool pstring::ends_with(const char *x)
{
    int len = count_cstring(x);
    if (len > length() || empty())
        return false;
    int starts_from = length() - len;
    for (int i = 0; i < len; i++) {
        if (_data[starts_from + i] != x[i])
            return false;
    }
    return true;
}

bool pstring::ends_with(char x) const noexcept
{
    return _data[_length - 1] == x;
}

pstring &pstring::replace(pstring::size_type pos, pstring::size_type count, const pstring &str)
{
    count = (count < str.length()) ? count : str.length();
    for (int i = 0; i < count; i++) {
        _data[pos + i] = str[i];
    }
    return *this;
}

pstring &pstring::replace(pstring::size_type pos, pstring::size_type count, const char *cstr)
{
    int len = count_cstring(cstr);
    count = (count < len) ? count : len;
    for (int i = 0; i < count; i++) {
        _data[pos + i] = cstr[i];
    }
    return *this;
}

pstring &pstring::replace(pstring::size_type pos, char ch)
{
    _data[pos] = ch;
    return *this;
}

pstring pstring::substr(pstring::size_type pos, pstring::size_type count) const
{
    count = ((count + pos) < length() - 1) ? count + pos: length() - 1;
    char orichar = _data[count];
    _data[count] = '\0';
    pstring ret(_data + pos);
    _data[count] = orichar;

    return ret;
}

pstring::size_type pstring::copy(char *dest, pstring::size_type count, pstring::size_type pos) const
{
    count = ((count + pos) < length()) ? count + pos: length();
    size_type r = 0;
    for (int i = 0; i < count; i++, r++) {
        dest[i] = _data[pos + i];
    }

    return r;
}

void pstring::resize(size_type count, char ch)
{
    if (count > capacity()) {
        int orisize = capacity();
        reserve(count);
        if (ch == '\0')
            return;
        for (int i = orisize; i < capacity(); i++) {
            _data[i] = ch;
        }
    } else {
        _capacity = count + DEFAULT_CAPACITY;
        char *store = new char[_capacity];
        for (int i = 0; i < count; i++) {
            store[i] = _data[i];
        }
        store[count] = '\0';
        delete[] _data;
        _data = store;
        _length = count;
    }
}

void pstring::swap(pstring &other) noexcept
{
    char *t_data = other._data;
    size_type t_length = other._length,
              t_capacity = other._capacity;
    
    other._data = _data;
    other._length = _length;
    other._capacity = _capacity;
    _data = t_data;
    _length = t_length;
    _capacity = t_capacity;
}

pstring pstring::operator+(const pstring &rhs)
{
    return pstring(*this).append(rhs);
}

pstring pstring::operator+(const char *rhs)
{
    return pstring(*this).append(rhs);
}

pstring pstring::operator+(const char rhch)
{
    return pstring(*this).append(rhch);
}

bool pstring::operator==(const pstring &other) const
{
    return compare(other) == 0;
}

bool pstring::operator!=(const pstring &other) const
{
    return compare(other) != 0;
}

bool pstring::operator>(const pstring &other) const
{
    return compare(other) > 0;
}

bool pstring::operator<(const pstring &other) const
{
    return compare(other) < 0;
}

bool pstring::operator>=(const pstring &other) const
{
    return compare(other) >= 0;
}

bool pstring::operator<=(const pstring &other) const
{
    return compare(other) <= 0;
}

bool pstring::operator==(const char *other) const
{
    return compare(other) == 0;
}

bool pstring::operator!=(const char *other) const
{
    return compare(other) != 0;
}

bool pstring::operator>(const char *other) const
{
    return compare(other) > 0;
}

bool pstring::operator<(const char *other) const
{
    return compare(other) < 0;
}

bool pstring::operator>=(const char *other) const
{
    return compare(other) >= 0;
}

bool pstring::operator<=(const char *other) const
{
    return compare(other) <= 0;
}

bool operator==(const char *cstr, const pstring &str) 
{
    return str.compare(cstr) == 0;
}

bool operator!=(const char *cstr, const pstring &str) 
{
    return str.compare(cstr) != 0;
}

bool operator>(const char *cstr, const pstring &str) 
{
    return str.compare(cstr) > 0;
}

bool operator<(const char *cstr, const pstring &str) 
{
    return str.compare(cstr) < 0;
}

bool operator>=(const char *cstr, const pstring &str) 
{
    return str.compare(cstr) >= 0;
}

bool operator<=(const char *cstr, const pstring &str) 
{
    return str.compare(cstr) <= 0;
}

std::ostream &operator<<(std::ostream& out, const pstring &str)
{
    return out.write(str.data(), str.length());
}

std::istream &operator>>(std::istream &in, pstring &str)
{
    char buf[1024];
    in >> buf;
    str.assign(buf);
    return in;
}

pstring::size_type pstring::find(const char *st, pstring::size_type from)
{
    auto strncmp = [](const char *a, const char *b, int n) -> bool
    {
        for (int i = 0; i < n; i++) {
            if (*a != *b)
                return false;
            a++, b++;
        }
        return true;
    };

    int len = count_cstring(st);
    for (int i = from; i < _length - len + 1; i++) {
        if (strncmp(_data + i, st, len))
            return i;
    }

    return npos;
}

pstring::size_type pstring::find(const pstring &st, pstring::size_type from)
{
    return find(st.data(), from);
}

pstring::size_type pstring::find(const char ch, pstring::size_type from)
{
    for (int i = from; i < _length; i++) {
        if (_data[i] == ch)
            return i;
    }

    return npos;
}

#endif // !__HEADER__PSTRING_