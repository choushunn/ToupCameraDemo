# ToupCameraDemo
ToupCameraDemo是一个使用 C++ 编写的项目，包括两个子模块"connx"和"cdevice"。

## 环境

要使用ToupCameraDemo，您需要具有以下环境：

- C++编译器
- CMake
- connx子模块所需的依赖项（请参阅connx子模块的说明）
- cdevice子模块所需的依赖项（请参阅cdevice子模块的说明）

## 基本功能

ToupCameraDemo的基本功能是实现某个功能，具体取决于connx和cdevice子模块的实现。请参阅各个子模块的说明以获取更多信息。

## 使用说明

要使用ToupCameraDemo，请按照以下步骤操作：

1. 克隆ToupCameraDemo的Git存储库：

   ````
   git clone git@github.com:choushunn/ToupCameraDemo.git
   
2. 初始化子模块：

   ````
   cd ToupCameraDemo
   git submodule init
   git submodule update
   ````

3. 使用CMake生成项目文件：

   ````
   mkdir build
   cd build
   cmake ..
   ````

4. 构建项目：

   ````
   make
   ````

5. 运行项目：

   ````
   ./ToupCameraDemo
   ````

请注意，具体的构建和运行步骤可能因您的环境而异。请根据您的实际情况进行调整。

## 部署

要部署ToupCameraDemo，您需要将可执行文件和所需的依赖项复制到目标计算机上。请注意，目标计算机上必须具有与ToupCameraDemo相同的环境才能运行它。

## 贡献指南

我们欢迎其他开发者为ToupCameraDemo做出贡献。如果您想要为项目做出贡献，请遵循以下准则：

- 在开始工作之前，请先查看当前的问题跟踪器以确保您的工作没有重复。
- 如果您想要实现新的功能，请先在问题跟踪器中提出请求以讨论该功能是否符合项目的目标。
- 如果您要提交代码，请先在本地创建一个新的分支，并从`master`分支中拉取最新的更改。
- 提交代码时，请附上适当的注释和文档。
- 如果您的代码更改影响了公共接口，请确保更新相应的文档和示例。
- 在提交代码之前，请确保您的代码符合项目的编码准则和代码风格。

## 如何贡献代码

我们欢迎您为本项目作出贡献！您可以通过以下步骤参与贡献：

1. 贡献者需要将主项目的代码 Fork 到自己的 GitHub 账号下。

2. Clone 自己的 GitHub 仓库到本地开发环境中：

```bash
git clone https://github.com/your-username/ToupCameraDemo.git
```

3. 贡献者创建一个新的本地分支，用于开发和修改：

```bash
git checkout -b develop
```

4. 在 develop 分支上进行代码开发和修改,将修改提交到 develop 分支，并推送到自己的 GitHub 仓库的 develop 分支上：

```bash
git add .
git commit -m "提交修改信息"
git push origin develop
```

5. 贡献者在自己的 fork 仓库页面上创建一个 Pull Request，将自己的 develop 分支合并到主项目的 develop 分支上。

6. 等待 Pull Request 被审核和合并。当 Pull Request 被合并后，您的修改将被包含在本仓库的 develop 分支中。

7. 主项目维护者将 develop 分支上的代码合并到 main 分支上，并推送到主项目的远程仓库

```bash
git checkout main
git pull origin main
git merge develop
git push origin main
```

8. 本地分支落后于远程分支解决方法。

```bash
# 拉取远程分支的最新代码。
git fetch
# 合并远程分支的最新代码到本地分支。
git merge origin/develop
# 解决合并冲突。提交合并后的代码，并推送到远程分支。
git push
```

注意：在贡献代码之前，请确保您的代码符合本项目的代码规范和最佳实践。如果您不确定如何贡献代码，请参考 GitHub 的 Pull Request 流程和相关文档，或者向项目维护者咨询和寻求帮助。同时，为了保证代码质量和稳定性，我们建议您在修改代码之前先进行测试和调试。

