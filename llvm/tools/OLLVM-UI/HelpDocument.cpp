#include "HelpDocument.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QRegularExpression>

HelpDocument::HelpDocument(QWidget *parent) : QDialog(parent) {
    setWindowTitle("帮助文档 - ALLVM 混淆编译配置工具");
    resize(900, 700);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    auto *textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);
    textEdit->setStyleSheet(
        "QTextEdit { background: #1a1a2e; color: #c0c0d0; border: none; font-size: 13px; }");
    textEdit->setHtml(mdToHtml(generateHelpContent()));
    layout->addWidget(textEdit);

    auto *btnClose = new QPushButton("关闭", this);
    btnClose->setFixedHeight(36);
    btnClose->setStyleSheet(
        "QPushButton { background: #3a3a5c; color: #e0e0e0; padding: 8px 30px; border-radius: 4px; font-size: 14px; }"
        "QPushButton:hover { background: #4a4a6c; }");
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(btnClose, 0, Qt::AlignCenter);
}

QString HelpDocument::generateHelpContent() {
    return R"MD(
# ALLVM 混淆编译配置工具

基于 LLVM 21.x 的 Android NDK 代码混淆一键配置工具，通过修改 `Android.mk` 自动注入编译标志实现混淆保护。

---

## 快速上手

### 第一步：选择项目目录

点击 **jni 文件夹** 右侧的 `选择` 按钮，选择你的 NDK 项目目录（包含 Android.mk 的 jni 文件夹或项目根目录）。

### 第二步：设置 NDK 路径

点击 `自动检测` 按钮，工具会自动查找同目录下的 NDK。如果未找到，请手动选择 NDK 根目录。

### 第三步：选择混淆功能

在 **混淆功能** 区域勾选需要的保护选项。各功能说明见下文。

### 第四步：注入 Android.mk

点击 **一键注入** 按钮，自动将混淆标志注入到 Android.mk 文件中。

### 第五步：一键编译

确认 NDK 路径正确后，点击 **一键编译 (ndk-build)** 开始编译。

### 第六步：收集产物

编译成功后，点击 **收集产物到输出** 将编译产物复制到指定的输出目录。

---

## 混淆功能详解

### 代码混淆

| 功能 | 参数 | 说明 |
|------|------|------|
| **控制流平坦化** | `-irobf-fla` | 将函数控制流平坦化，增加逆向难度 |
| **间接分支** | `-irobf-indbr` | 将直接跳转改为间接跳转 (L1-L3) |
| **间接调用** | `-irobf-icall` | 将直接调用改为间接调用 (L1-L3) |
| **全局变量间接化** | `-irobf-indgv` | 将全局变量访问改为间接访问 (L1-L3) |
| **字符串加密** | `-irobf-cse` | 加密字符串常量，运行时解密 |

### 常量加密

| 功能 | 参数 | 说明 |
|------|------|------|
| **常量整数加密** | `-irobf-cie` | 加密整数常量 (L1-L3) |
| **常量浮点加密** | `-irobf-cfe` | 加密浮点数常量 (L1-L3) |

### 反调试/检测

| 功能 | 参数 | 说明 |
|------|------|------|
| **RTTI 擦除** | `-irobf-rtti` | 移除 C++ RTTI 类型信息 |
| **系统调用保护** | `-irobf-syscall` | 替换 libc 函数为直接 syscall，防止 Hook |
| **内存保护** | `-irobf-memprotect` | 内存 Dump 保护 |
| **内存Dump & Maps保护** | `-irobf-memmaps` | 组合保护：禁用内存Dump + 隐藏Maps + 伪造Maps |
| **LD_PRELOAD 检测** | `-irobf-ldpreload` | 检测 LD_PRELOAD 注入 |
| **虚拟机检测** | `-irobf-vmdetect` | 检测是否在虚拟机中运行 |
| **USB 调试保护** | `-irobf-usb` | 禁用 USB 调试 |
| **IDA 调试器检测** | `-irobf-ida` | 检测 IDA Pro 调试器 |
| **VPN 检测** | `-irobf-vpn` | 检测 VPN 连接 |
| **代理/iptables 检测** | `-irobf-proxy` | 检测代理和 iptables |
| **时间差检测** | `-irobf-time` | 检测时间差调试 |
| **Hosts 文件检测** | `-irobf-hosts` | 检测 Hosts 文件篡改 |
| **内存检测** | `-irobf-mem` | 内存驻留检测 |
| **Ptrace 检测** | `-irobf-ptrace` | 检测 Ptrace 调试器 |
| **Root 检测** | `-irobf-root` | 检测 Root 环境（有 Root 则退出） |
| **非 Root 检测** | `-irobf-noroot` | 检测非 Root 环境（无 Root 则退出） |

### 其他功能

| 功能 | 参数 | 说明 |
|------|------|------|
| **A-Protect 输出** | `-irobf-aprotect` | 输出 A-Protect 保护信息 |
| **调试日志** | `-irobf-debug` | 输出混淆调试信息 |

---

## VMP 虚拟机保护

VMP 是最高强度的代码保护，将代码转换为自定义虚拟机指令执行。

> **重要**: VMP 要求禁用C++异常处理，UI会自动注入 `-fno-exceptions -frtti` 编译选项。

### 启用方式

**方式1：使用注解（推荐）**

```cpp
__attribute__((annotate("vmp")))
int my_protected_function(int a, int b) {
    return a + b;
}
```

**方式2：使用命令行参数**

```bash
-mllvm -irobf-vmp
-mllvm -irobf-vm_functions=func1;func2;func3
```

**方式3：在 Android.mk 中添加**

```makefile
LOCAL_CFLAGS += -mllvm -irobf-vmp
LOCAL_CFLAGS += -fno-exceptions -frtti
```

---

## 系统调用保护说明

系统调用保护会将以下 libc 函数替换为直接 syscall：

| 原函数 | 系统调用号 | 说明 |
|--------|------------|------|
| `connect` | 203 | Socket 连接 |
| `send` / `sendto` | 206 | 发送数据 |
| `recv` / `recvfrom` | 207 | 接收数据 |
| `read` | 63 | 读取数据 |
| `write` | 64 | 写入数据 |
| `clock_gettime` | 223 | 获取时间 |

---

## 混淆强度等级

部分功能支持 L1/L2/L3 三个等级：

- **L1**：轻度混淆，性能影响小
- **L2**：中度混淆，平衡性能和保护
- **L3**：重度混淆，最高保护强度

---

## 注意事项

1. **备份**：注入前会自动创建 Android.mk.bak 备份文件
2. **兼容性**：部分功能仅支持 ARM64 架构
3. **性能**：混淆会增加代码体积和运行开销，请按需选择
4. **VMP**：VMP 保护会显著增加代码体积，仅对关键函数使用。需要禁用异常处理（-fno-exceptions）
5. **Root 检测**：Root 检测和非 Root 检测互斥，只能选择其一
6. **A-Protect**：A-Protect 输出现在默认启用，在程序启动时打印保护信息

---

## 快捷操作

- `刷新`：重新加载 Android.mk 文件
- `清理控制台`：清空编译输出日志
- `停止编译`：强制终止正在进行的编译

---

## 作者信息

- **作者**：abcdefgjh
- **QQ**：3986612313
- **TG**：@abcdefgjha
- **GitHub**：https://github.com/abcdefgjh-li/ALLVM

)MD";
}

QString HelpDocument::mdToHtml(const QString &md) {
    QStringList lines = md.split('\n');
    QString html;
    html += "<style>"
            "body { font-family: 'Microsoft YaHei', 'Segoe UI', sans-serif; font-size: 13px; line-height: 1.7; margin: 8px; }"
            "h1 { color: #00d4aa; font-size: 20px; border-bottom: 2px solid #3a3a5c; padding-bottom: 8px; margin-top: 16px; }"
            "h2 { color: #3498db; font-size: 16px; border-bottom: 1px solid #3a3a5c; padding-bottom: 6px; margin-top: 20px; }"
            "h3 { color: #e67e22; font-size: 14px; margin-top: 16px; }"
            "table { border-collapse: collapse; width: 100%; margin: 12px 0; }"
            "th { background: #252540; color: #00d4aa; padding: 8px 10px; text-align: left; border: 1px solid #3a3a5c; }"
            "td { padding: 6px 10px; border: 1px solid #3a3a5c; }"
            "tr:nth-child(even) { background: #1e1e32; }"
            "code { background: #252540; color: #00ff88; padding: 2px 6px; border-radius: 3px; font-family: Consolas, 'Courier New', monospace; font-size: 12px; }"
            "pre { background: #12122a; border: 1px solid #3a3a5c; border-radius: 6px; padding: 12px; overflow-x: auto; margin: 10px 0; }"
            "pre code { background: transparent; padding: 0; }"
            "p { color: #c0c0d0; margin: 8px 0; }"
            "hr { border: none; border-top: 1px solid #3a3a5c; margin: 16px 0; }"
            "strong { color: #00d4aa; }"
            "li { color: #c0c0d0; margin: 4px 0; }"
            "</style>";

    bool inCodeBlock = false;
    QString codeBlock;

    for (const auto &line : lines) {
        QString processed = line;

        if (processed.startsWith("```")) {
            if (inCodeBlock) {
                html += "<pre><code>" + codeBlock.toHtmlEscaped() + "</code></pre>";
                codeBlock.clear();
                inCodeBlock = false;
            } else {
                inCodeBlock = true;
            }
            continue;
        }

        if (inCodeBlock) {
            if (!codeBlock.isEmpty()) codeBlock += "\n";
            codeBlock += processed;
            continue;
        }

        processed = processed.toHtmlEscaped();

        if (processed.startsWith("---")) {
            html += "<hr>";
        } else if (processed.startsWith("### ")) {
            html += "<h3>" + processed.mid(4) + "</h3>";
        } else if (processed.startsWith("## ")) {
            html += "<h2>" + processed.mid(3) + "</h2>";
        } else if (processed.startsWith("# ")) {
            html += "<h1>" + processed.mid(2) + "</h1>";
        } else if (processed.startsWith("- ")) {
            html += "<li>" + processed.mid(2) + "</li>";
        } else if (processed.startsWith("| ")) {
            static bool inTable = false;
            if (!inTable) {
                html += "<table>";
                inTable = true;
            }
            if (processed.contains("---")) {
                continue;
            }
            QStringList cells = processed.split("|");
            html += "<tr>";
            for (const auto &cell : cells) {
                QString c = cell.trimmed();
                if (!c.isEmpty()) {
                    html += "<td>" + c + "</td>";
                }
            }
            html += "</tr>";
        } else if (!processed.isEmpty()) {
            static bool inTable = false;
            if (inTable) {
                html += "</table>";
                inTable = false;
            }
            QRegularExpression codeRegex("`([^`]+)`");
            processed.replace(codeRegex, "<code>\\1</code>");
            QRegularExpression boldRegex("\\*\\*([^*]+)\\*\\*");
            processed.replace(boldRegex, "<strong>\\1</strong>");
            html += "<p>" + processed + "</p>";
        }
    }

    return html;
}
