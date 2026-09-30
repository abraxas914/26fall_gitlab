# Lab0: GitLab 实验报告

> **姓名**：（填写姓名）　**学号**：（填写学号）　**日期**：2026-09-30
>
> 仓库链接：（填写你的个人仓库 URL，如 `https://github.com/<username>/26fall_gitlab`）

---

## 一、文档问题回答

### 1. 你之前有过多人协同开发的经历吗？如果有，你们是使用什么方式分工协作的？

> ⚠️ 以下为按常见情况代写的版本，请按你的真实经历修改。

有过。之前在程序设计课程的大作业中，我们小组三个人共同完成一个项目。最初我们用的是最原始的方式：微信群互传压缩包，文件名靠 `final_v2_真最终版.zip` 区分——结果某次有人改错了版本覆盖了别人的代码，大家花了一晚上才找回丢失的修改。

后来我们迁移到 GitHub 协作：仓库建在组长账号下，按模块分工，每人从 `main` 分支切出自己的分支（如 `feature-parser`），完成后发起 Pull Request，由另一位同学 review 之后再合并。遇到需要同时改同一个文件的情况，就先 `git pull` 再提交。这次的教训让我切身体会到：**分工方式决定了协作成本，而版本控制工具的存在就是为了把这个成本降到最低。**

### 2. 思考一下，Git 为什么要设计"暂存-提交"两个步骤？

我理解主要有以下几点原因：

**（1）暂存区是一次提交的"草稿快照"，让开发者能够组装出逻辑完整的提交。**
工作目录里的改动往往是碎片化的：可能同时改了一个 bug、顺手调了下格式、又删了几行调试代码。如果直接全部提交，就会产生一个"什么都有一点"的脏提交。有了暂存区，可以用 `git add <file>` 挑选文件、甚至 `git add -p` 挑选某几行改动，把相关的修改组装成若干个各自独立的、语义清晰的提交。提交历史因此成为**可以被逐条阅读和回滚的项目编年史**，而不是一堆大杂烩。

**（2）暂存区提供了提交前的最后检查点。**
`git diff` 看的是"工作区改了什么"，`git diff --staged` 看的是"即将被提交什么"。后者让你在按下"确定"之前，还能完整审查这次提交的真实内容，避免把密钥、调试代码等不希望入库的东西提交进去。

**（3）历史渊源：Git 是为管理 Linux 内核的补丁流而设计的。**
内核开发的协作模式就是"把待合入的修改整理成一个 patch，再由维护者审阅合入"。暂存区（index）正对应这个"整理 patch"的动作——`git diff --staged` 的输出就是一份可以直接发给别人审阅的补丁。暂存-提交的两段式设计本质上是把这套补丁工作流内建进了版本控制系统。

一句话总结：**工作区是你随心所欲的地方，暂存区是你郑重其事组装提交的地方，仓库是最终定稿的地方**——两步设计在"随意修改"和"正式记录"之间设置了一道有意识的缓冲。

### 3. `git branch` 和 `git branch -a` 的区别是什么？

- `git branch` 列出**本地分支**，并用 `*` 标出当前所在分支。例如：
  ```
    feature
  * main
  ```

- `git branch -a` 列出**本地分支 + 远程跟踪分支**（remote-tracking branches），例如：
  ```
    feature
  * main
    remotes/origin/HEAD -> origin/main
    remotes/origin/main
  ```

远程跟踪分支（形如 `remotes/origin/xxx`）是本地缓存的远程分支状态，记录的是**上一次 `git fetch` / `git pull` / `git clone` / `git push` 时**远程仓库的样子，并不是实时的。这也是为什么有时远程分支已经被别人删了，本地 `git branch -a` 还能看到——需要用 `git fetch --prune` 同步。

一个常见用途：当你想确认"远程是否存在某个分支"（比如 `remotes/origin/feature`）时，就需要用 `-a`（或 `--all`）；仅用 `git branch` 是看不到的。

---

## 二、实验步骤

> 以下命令均在本仓库目录下执行。标注 📷 的位置需要截图。

### 0. 环境配置

```bash
git --version        # 📷 截图：确认 git 安装成功
git config --global user.name  "<用户名>"
git config --global user.email "<邮箱>"
```

（如使用 SSH，还需要 `ssh-keygen` 生成密钥并在 GitHub 添加公钥，用 `ssh -T git@github.com` 验证。）

### 1. 完成并提交 main.c（任务 2，50 分）

`main.c` 的 TODO 部分，我选择打印 Anthropic 前沿红队于 2026-09-29 发布的安全评估报告《GLM-5.3 and the spread of advanced cyber capabilities》的核心发现——这份报告讨论了 GLM-5.3 在缺乏防滥用保障措施的情况下发布、及其自主构建端到端网络攻击能力的问题，共 8 条要点：核心结论、攻击能力、ExploitBench 基准成绩、0-day 漏洞挖掘实测、CVE-2026-11645 的 N-day 攻击、防护绕过比例、NIST CAISI 评估以及总体判断。

编译验证（README 给出的方式）：

```bash
make && ./main && make clean     # 📷 截图：编译并运行成功
```

提交修改：

```bash
git add main.c
git commit -m "feat: print key findings of GLM-5.3 cyber capabilities report"
git push                          # 📷 截图：commit 与 push 成功
```

![image-20260930225842202](/Users/ethan/Library/Application Support/typora-user-images/image-20260930225842202.png)



### 2. 分支管理与合并冲突（任务 4，10 + 10 分）

**Step 1：新建并切换到 feature 分支**

```bash
git switch -c feature            # 📷 截图：分支创建成功
```

**Step 2：在 feature 分支上修改 main.c 并提交**

把 `main.c` 中第 [3] 条 printf 改成（例如）：

```c
    printf("[3] ExploitBench (V8 engine exploitation): GLM-5.3 built 50 "
           "end-to-end exploits in 410 attempts, a success rate of about "
           "12 percent.\n");
```

```bash
git add main.c
git commit -m "modify finding [3] in feature branch"
```

**Step 3：切回 main 分支，修改 main.c 的同一位置并提交**

把 `main.c` 中**同一句**第 [3] 条 printf 改成**不同的**内容（例如）：

```c
    printf("[3] ExploitBench benchmark: GLM-5.3 solved 50 out of 410 V8 "
           "exploitation tasks (~12%), far ahead of GLM-5.2 and Kimi K3.\n");
```

```bash
git add main.c
git commit -m "modify finding [3] in main branch"
```

> 关键点：两次提交修改的是**同一文件的同一处位置**，这样 merge 时 Git 无法自动决定保留哪个版本，必然产生冲突。

**Step 4：合并并解决冲突**

```bash
git merge feature                 # 📷 截图：出现 CONFLICT 提示
```

Git 会提示 `CONFLICT (content): Merge conflict in main.c`，此时打开 main.c 可以看到：

```c
<<<<<<< HEAD
    printf("[3] ExploitBench benchmark: GLM-5.3 solved 50 out of 410 ...");
=======
    printf("[3] ExploitBench (V8 engine exploitation): GLM-5.3 built 50 ...");
>>>>>>> feature
```

手动编辑，把冲突标记删掉、保留（或融合）想要的版本，然后：

```bash
make && ./main && make clean      # 确认解决冲突后代码仍能编译运行
git add main.c
git commit                        # 完成合并提交 📷 截图：冲突已解决
git log --oneline --graph --all   # 📷 截图：分支合并历史
git push
```

### 3. 提交实验报告（任务 5）

```bash
git add REPORT.md
git commit -m "docs: add lab0 report"
git push                          # 📷 截图：报告提交成功
```

---

## 三、阅读材料概括（任选其二）

我选择的材料是《Commit Message 规范》和《Git Flow 分支控制》。

### 1. Commit message 规范（阮一峰）

文章介绍了目前社区使用最广的 **Angular 规范**。核心要点：

**格式**：一条 commit message 由三部分构成——Header、Body、Footer：

```
<type>(<scope>): <subject>
// 空一行
<body>
// 空一行
<footer>
```

- **Header（必需）**：`type` 是提交类别，只允许 7 种标识——`feat`（新功能）、`fix`（修补 bug）、`docs`（文档）、`style`（格式）、`refactor`（重构）、`test`（测试）、`chore`（构建/辅助工具）；`scope` 是影响范围（可选）；`subject` 是不超过 50 字符的简短描述，动词开头、首字母小写、结尾不加句号。
- **Body（可选）**：详细说明改动的**动机**以及与旧行为的**对比**。
- **Footer（可选）**：两种用途——标注不兼容变动（`BREAKING CHANGE:` 开头）和关闭 Issue（`Closes #123`）。
- **Revert**：撤销提交必须以 `revert:` 开头，Body 固定写 `This reverts commit <hash>.`。

**为什么值得遵守**：格式化的 commit message 有三大好处——① 用 `git log --pretty=format:%s` 一行一条快速浏览历史；② 用 `git log --grep feature` 过滤特定类别提交；③ 可以直接用 conventional-changelog 等工具**自动生成 Change Log**。文中还介绍了 Commitizen（用 `git cz` 交互式生成规范 message）和 validate-commit-msg（在 commit-msg 钩子中强制校验）等配套工具。

一句话概括：**commit message 不是写给自己看的备注，而是结构化、可检索、可再加工的项目数据。**

### 2. Git Flow 分支控制

文章介绍了经典的 **Gitflow** 分支管理模型，它围绕几类各司其职的分支组织开发流程：

| 分支 | 作用 | 生命周期 |
|------|------|----------|
| `master` | 与线上运行版本一致，每个 commit 打 tag | 永久 |
| `develop` | 开发集成分支，从 master 分离出来 | 永久 |
| `feature/*` | 从 develop 创建，开发单个新功能，用 `--no-ff` 合回 develop 后删除 | 临时 |
| `release/*` | 从 develop 创建进入提测阶段，修复测试 Bug，最终合并到 master 和 develop，打 tag 后删除 | 临时 |
| `hotfix/*` | 线上出问题时从 master/tag 创建紧急修复，合并回 master 和 develop 后删除 | 临时 |

其核心原则是"**从哪里来，最后回到哪里去**"：feature 从 develop 分离就合回 develop，hotfix 从 master 分离就同时回到 master 和 develop，保证各分支语义不被污染。文中还给出了版本号规则（末位为热修复、中位为功能迭代、首位为重大变更，形如 `0.0.0`）以及 `git flow` 扩展库的用法。

我的理解：Gitflow 的价值在于用**分支的职责划分**替代了"所有人往一个分支上堆代码"的混乱——feature 分支隔离了每个人的开发现场，release 分支隔离了不稳定的测试版本，master 永远可信。虽然现代很多团队用更轻量的 GitHub Flow（只有 main + 短生命周期分支），但"分支各司其职"的思想源头都是 Gitflow。

---

## 四、为什么要学习 Git

结合本次实验和上面的阅读材料，我对"为什么要学习 Git"的理解是：

**第一，Git 是程序员的"时间机器"。** 每一次 commit 都是一个可以随时回去的存档点。没有版本控制时，"改坏了但不知道改了什么"是灾难；有了版本控制，任何一次回归都可以被 `git diff` 定位、被 `git revert` / `git reset` 撤销。实验里 `git log --oneline` 能逐条回放我今晚做的每一步操作，这种确定性本身就是生产力。

**第二，Git 让多人协作从"互相覆盖"变成"并行推进"。** 分支的本质是给每个人一个独立的工作现场，互不干扰，最后再通过 merge 汇合。本次实验制造并解决合并冲突的过程，正是真实协作的缩影——两个人同时改了同一处代码，Git 不会悄悄丢掉任何一方的修改，而是明确地停下来要求人来做决定。**冲突处理机制的意义不在于制造麻烦，而在于把"谁覆盖了谁"这种不可见的灾难，变成一次必须显式沟通的对话。**

**第三，Git 是工业界的通行语言。** 从 Gitflow 到 GitHub Flow，几乎所有企业的研发流程都构建在 Git 分支模型之上；CI/CD、Code Review、发布管理全部以 Git 为地基。不会 Git，就意味着无法参与任何现代软件团队的工作流程。

**第四，规范化的 Git 使用让项目历史成为文档。** 正如 Commit Message 规范一文所展示的：结构化的提交信息可以被检索、被过滤、被自动转换成 Change Log。一个 commit history 维护良好的仓库，本身就是最好的项目编年史——这也解释了为什么本实验要求我们从第一次提交就养成写清晰 commit message 的习惯。

---

## 五、建议（可选）

- 文档可以在"任务 4"部分提前说明"修改同一文件的同一位置会产生冲突"的原理（共同祖先 + 双方修改 = 冲突），不熟悉的话第一次 merge 遇到 `<<<<<<<` 标记会有点慌。
- 可以在文档中补充推荐 `git log --oneline --graph --all` 这样的"可视化"命令，直观感受分支的合并历史。
