# 模块 3: OOP 对比 — Java vs C++

## 核心差异

| 维度 | Java | C++ |
|------|------|-----|
| 类默认可见性 | package-private | private |
| 构造逻辑 | 只初始化字段 | 初始化列表 + 构造函数体 |
| 析构 | GC finalize() (不可靠) | 析构函数（确定性的） |
| 继承 | 单继承 + 多接口 | 多重继承（含虚继承） |
| 方法绑定 | 默认虚（可被重写） | 默认非虚（必须声明 virtual） |
| 抽象 | abstract class / interface | 纯虚函数 `= 0` |
| 访问控制 | public/protected/private/default | public/protected/private |
| 友元 | 无 | friend 类/函数 |
| 注解 vs 关键字 | @Override | override 关键字(C++11) |

## 知识点: 构造与析构

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| 初始化顺序 | 字段默认值 → 构造体 | 基类 → 成员声明顺序 → 构造体 | ⚠️ 初始化列表顺序必须与声明顺序一致 |
| 析构顺序 | 不可预测(GC) | 构造的逆序(确定的) | ⚠️ 析构函数绝对不能抛异常 |
| 拷贝构造 | Cloneable / clone() | 拷贝构造函数 | ⚠️ C++ 默认生成浅拷贝 |
| 基类构造中调用虚函数 | 调用重写版本(危险) | 调用基类版本(安全) | ⚠️ C++ 构造期间虚表指向基类 |

### 相关编码规范

| 规范来源 | 编号 | 规则 |
|----------|------|------|
| C++ Core Guidelines | C.21 | 三五法则 |
| C++ Core Guidelines | C.35 | 基类的析构函数必须是 public 和 virtual |
| C++ Core Guidelines | C.47 | 按成员声明顺序定义和初始化成员变量 |
| C++ Core Guidelines | C.48 | 优先用类内初始化器 |
| C++ Core Guidelines | C.128 | 虚函数应该指定 virtual/override/final 三者之一 |
| C++ Core Guidelines | C.129 | 设计类层次时区分实现继承和接口继承 |
| Google Style Guide | - | 如果类可能被继承，析构函数必须是 virtual |

## 知识点: 虚函数与多态

| 场景 | Java | C++ | 踩坑提醒 |
|------|------|-----|----------|
| 声明多态方法 | 默认虚 | 显式 `virtual` 声明 | ⚠️ 忘记 virtual = 静态绑定 |
| 覆写标记 | `@Override` 注解 | `override` 关键字 | ⚠️ 不用 override 就不会检查签名匹配 |
| 阻止覆写 | `final class/method` | `final` | 语义相同 |
| 调用基类方法 | `super.method()` | `Base::method()` | ⚠️ 语法不同 |
| 析构函数 | 无 | 基类析构必须 `virtual` | ⚠️ 非虚析构 + delete 基类指针 = UB |

## 知识点: final / override / = 0

| C++ | 等价 Java | 说明 |
|-----|-----------|------|
| `virtual void foo() = 0;` | `abstract void foo();` | 纯虚函数 |
| `void foo() override;` | `@Override void foo()` | 覆写 |
| `void foo() final;` | `final void foo()` | 禁止进一步覆写 |
| `class Bar final {};` | `final class Bar {}` | 禁止继承 |

## 常见 Bug（Code Review 回退清单）

- [ ] 基类析构函数不是 virtual
- [ ] 忘了写 override
- [ ] 析构函数中抛异常
- [ ] 初始化列表顺序和成员声明顺序不一致
- [ ] 在构造函数中调用 this 的虚函数
