#include"pstring.hpp"
#define show(cont)              cout << #cont << " = " << (cont) << endl;
#define show_text(text, cont)   cout << text << ", " << #cont << " = " << (cont) << endl;
#define show_bool(cont)         cout << #cont << ": " << (cont ? "true" : "false") << endl;
#define show_before(act, show)  cout << "执行 " << #act << " 之前, " << #show << " = " << (show) << endl;
#define show_after(act, show)   (act); cout << "执行 " << #act << " 之后, " << #show << " = " << (show) << endl;
#define show_ba(act, show)      show_before(act, show); show_after(act, show);
#define section(name)           cout << "========== " << #name << " ==========" << endl;
#define subsection(name)        cout << "> " << #name << " <" << endl;
#define begin                   {
#define end                     cout << endl; }

using namespace std;

int main(int argc, char *argv[])
{
    pstring s1 = "Pangda\'s string", // 支持直接使用字符串字面量或C风格字符串赋值
            s2,                      // 默认构建空字符串
            s3 = s1,                 // 拷贝赋值
            s4('a', 20);             // 重复字符构建字符串

    pstring t1 = "bb",
            t2('b', 2),
            t3 = "aa";

    section(输入输出)
    begin
        subsection(使用cin读入字符串)
        begin
            cin >> s2; 
        end

        subsection(使用cout输出字符串)
        begin
            show(s1);
            show(s2);
            show(s3);
            show(s4);
        end
    end

    section(赋值)
    begin
        subsection(使用assign函数)
        begin
            show_after(s4.assign('a'), s4);
            show_after(s4.assign("aaa"), s4);
            show_after(s4.assign('a', 6), s4);
        end

        subsection(使用等号)
        begin
            show_after(s4 = 'a', s4);
            show_after(s4 = "aaa", s4);
        end
    end
    
    section(获得指定位置上的字符)
    begin
        subsection(使用at函数)
        begin
            try
            begin
                // at函数将执行边界检查, 若不在界限内, 将抛出std::out_of_range异常
                show(s1.at(100));
            end
            catch (exception ex)
            begin
                cout << "发生了错误: " << ex.what() << endl;
            end
            show(s1.at(4));
        end

        subsection(使用[])
        begin
            show(s1[3]);
        end

        subsection(查找)
        begin
            show(s1.find("gda"));
            show(s1.find("emmm"));
        end
    end

    section(基本的容器函数)
    begin
        subsection(头尾元素)
        begin
            show(s1.front());
            show(s1.back());
        end

        subsection(容器容量)
        begin
            show_bool(s1.empty());
            show(s1.size());
            show(s1.length());
        end

        subsection(清除内容)
        begin
            show_ba(s4.clear(), s4);
        end

        subsection(栈相似性)
        begin
            show_after(s3.push_back('!'), s3);
            show_after(s3.pop_back(), s3);
        end
    end

    section(字符串容器专有函数)
    begin
        subsection(内存存储)
        begin
            show(s1.data());
            show(s1.c_str());
        end

        subsection(库支持性)
        begin
            show(s1.max_size());
        end

        subsection(插入删除)
        begin
            show_ba((s3.insert(3, "Hello"),
                     s3.insert(1, s1)), s3);
            show_after(s3.erase(1, s1.length()), s3)
        end

        subsection(追加字符)
        begin
            pstring tmp = "bb";
            show(tmp);
            show_after(s3.append("aa"), s3);
            show_after(s3.append(tmp), s3);

            show_after(s3 += "aa", s3);
            show_after(s3 += tmp, s3);
        end

        subsection(字符串判定)
        begin
            show(t1);
            show(t2);
            show(t3);
            show_bool(t1 == t2);
            show_bool(t1 != t2);
            show_bool(t1 > t2);
            show_bool("aa" == t3);
        end

        subsection(前缀后缀判定)
        begin
            show(s1);
            show_bool(s1.starts_with('P'));
            show_bool(s1.ends_with("str"));
        end

        subsection(字符替换)
        begin
            show_after(s1.replace(0, 1, 'p'), s1);
        end

        subsection(子串)
        begin
            show(s1.substr(3, 7));
        end

        subsection(复制到字符数组)
        begin
            char buff[100];
            show_after(s1.copy(buff, 100), buff);
        end

        subsection(交换字符串)
        begin
            cout << "执行 t1.swap(t3) 之前, t1 = " << t1 << ", t3 = " << t3 << endl;
            t1.swap(t3);
            cout << "执行 t1.swap(t3) 之后, t1 = " << t1 << ", t3 = " << t3 << endl;
        end

        subsection(链接字符串)
        begin
            show(t1 + t2);
            show(t1 + "nihao");
        end

        subsection(设置容器大小)
        begin
            show_after(s1.resize(5), s1);
        end

        subsection(设置预留空间)
        begin
            show_after(s2.reserve(100), s2.capacity());
            show_after(s2.shrink_to_fit(), s2.capacity());
        end
    end

    section(迭代器支持)
    begin
        subsection(Range-based for)
        begin
            cout << "使用Range-based for输出的结果: s2 = ";
            for (auto i: s2) 
            begin
                cout << i;
            end
            cout << endl;
        end

        subsection(普通迭代器)
        begin
            cout << "使用逆向迭代器输出的结果: s2 = ";
            for (auto i = s2.cbegin(); i != s2.cend(); ++i)
            begin
                cout << *i;
            end
            cout << endl;
        end
    end

    return 0;
}