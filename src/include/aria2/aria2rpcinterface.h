#ifndef ARIA2RPCINTERFACE_H
#define ARIA2RPCINTERFACE_H

#include <QObject>
#include <QJsonObject>
#include "aria2cbtinfo.h"
#include "aria2const.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class Aria2RPCInterface : public QObject
{
    Q_OBJECT
public:
    explicit Aria2RPCInterface(QObject *parent = nullptr);

public:

    /**
     *@brief startUp 启动aria2c进程
     *@return 启动成功true  启动失败 false
     */
    bool startUp();
    /**
     *@brief init 初始化
     *@return
     */
    void init();//初始化RPC服务
    /**
     *@brief checkAria2cProc() 检测aria2c是否启动
     *@return 启动成功 true  否则 false
     */
    bool checkAria2cProc();

    /**
     *@brief killAria2cProc关闭aria2c进程
     *@return 启动成功 true  否则 false
     */
    bool killAria2cProc();

    /**
     *@brief 设置默认的下载路径
     *@param strDir
     *@return
     */
    void setDefaultDownLoadDir(QString strDir);

    /**
     *@brief 获得默认的下载路径
     *@return
     */
    QString getDefaultDownLoadDir() ;//{return this->defaultDownloadPath();}

    /**
     *@brief 设置配置文件路径
     *@param strPath
     *@return
     */
    void setConfigFilePath(QString strPath);

    QString getConfigFilePath();//获得配置文件路径

    bool checkAria2cFile();//检查aria2c文件

    void addUri(QString strUri,QMap<QString,QVariant> opt,QString strId);//添加uri地址

    void addTorrent(QString strTorrentFile,QMap<QString,QVariant> opt,QString strId);//添加bt文件

    void addMetalink(QString strMetalink,QMap<QString,QVariant> opt,QString strId);//添加磁力链地址

    void pause();//暂停
    void pauseAll();//暂停所有
    void unPause();//取消暂停
    void unPauseAll();//取消所有暂停
    void remove();//移除
    /**
     * @brief tellStatus 获取下载状态
     * @param gId GID aria2c生成的下载唯一标示
     * @param id 同其它方法参数
     *
     * signal_success信号中异步返回指定下载的当前状态
     * result为一个JsonObject，包含类似map键值，详见aria2文档
     */
    void tellStatus(QString gId, QString id = "");

    /**
     * @brief tellStatus 获取下载状态 可指定需要的返回字段
     * @param gId GID aria2c生成的下载唯一标示
     * @param keys 指定需要的返回字段列表，参数详见aria2文档
     * [status|totalLength|completedLength|uploadLength|bitfield|downloadSpeed|uploadSpeed|...]
     * 示例代码：
     * QStringList keys;
     * keys<<"gid"<<"status"<<"totalLength"<<"completedLength"<<"downloadSpeed";
     * @param id 同其它方法参数
     *
     * signal_success信号中异步返回指定下载的当前状态
     * result为一个JsonObject，包含类似map键值，详见aria2文档
     */
    void tellStatus(QString gId, QStringList keys, QString id = "");
    /**
     * @brief pause 暂停指定下载 active/waiting状态将变为paused
     * @param gId GID
     * @param id 可选，该参数用来唯一标示一次请求，异步返回的结果里会包含请求发出是指定的id，用以区分匹配多次请求。
     * 默认为method名，详见aria2cconst.h常量
     *
     * signal_success信号中异步返回本次下载的GID
     * {id:"", jsonrpc:"2.0", result:"gid"}
     * QString gId = json.value("result").toString();
     */
    void pause(QString gId, QString id = "");

    /**
     * @brief forcePause 强制暂停 active/waiting状态将变为paused
     * @param gId GID aria2c生成的下载唯一标示
     * @param id 可选，该参数用来唯一标示一次请求，异步返回的结果里会包含请求发出是指定的id，用以区分匹配多次请求。
     * 默认为method名，详见aria2cconst.h常量
     *
     * signal_success信号中异步返回本次下载的GID
     * {id:"", jsonrpc:"2.0", result:"gid"}
     * QString gId = json.value("result").toString();
     */
    void forcePause(QString gId, QString id = "");
    /**
     * @brief pauseAll 全部暂停 active/waiting状态将变为paused
     * @param id 同其它方法参数
     *
     * signal_success信号中异步返回 "OK"
     * {id:"", jsonrpc:"2.0", result:"ok"}
     * QString ok = json.value("result").toString();
     */
    void pauseAll(QString id = "");//

    /**
     * @brief forcePauseAll 强制全部暂停
     * @param id 同其它方法参数
     *
     * signal_success信号中异步返回 "OK"
     * {id:"", jsonrpc:"2.0", result:"ok"}
     * QString ok = json.value("result").toString();
     */
    void forcePauseAll(QString id = "");
    /**
     * @brief unpause 恢复指定GID下载 paused状态变为waiting
     * @param gId GID aria2c生成的下载唯一标示
     * @param id 同其它方法参数
     *
     * signal_success信号中异步返回本次下载的GID
     * {id:"", jsonrpc:"2.0", result:"gid"}
     * QString gId = json.value("result").toString();
     */
    void unpause(QString gId, QString id = "");

    /**
     * @brief unpauseAll 全部恢复下载
     * @param id 同其它方法参数
     *
     * signal_success信号中异步返回 "OK"
     * {id:"", jsonrpc:"2.0", result:"ok"}
     * QString ok = json.value("result").toString();
     */
    void unpauseAll(QString id = "");

    /**
     * @brief remove 移除指定下载。下载状态会变为removed。
     * 注意，实验表明并不会删除下载的文件。
     * @param gId GID aria2c生成的下载唯一标示
     * @param id 同其它方法参数
     *
     * signal_success信号中异步返回本次下载的GID
     * {id:"", jsonrpc:"2.0", result:"gid"}
     * QString gId = json.value("result").toString();
     */
    void remove(QString gId, QString id = "");

    /**
     * @brief forceRemove 强制移除,不会做后处理
     * @param gId GID aria2c生成的下载唯一标示
     * @param id 同其它方法参数
     *
     * signal_success信号中异步返回本次下载的GID
     * {id:"", jsonrpc:"2.0", result:"gid"}
     * QString gId = json.value("result").toString();
     */
    void forceRemove(QString gId, QString id = "");


private:
    /**
     *@brief 调用RPC
     *@param method 调用的方法 params  该方法对应的参数   id
     *
     *@return
     */
    void callRPC(QString method, QJsonArray params, QString id = "");
    void callRPC(QString method, QString id = "");//

    /**
     *@brief 发送请求
     *@param jsonObj json包 method请求的方法
     *
     *@return
     */
    void sendMessage(QJsonObject jsonObj,const QString &method);
    /**
     *@brief 请求的返回
     *@param reply网络对象 method 方法 id
     *
     *@return
     */
    void rpcRequestReply(QNetworkReply *reply,const QString &method,const QString id);
private:
    //QString cmd = "/usr/bin/aria2c";//aria2c程序路径 -> 已改成public static变量
    QString rpcPort = "88888";//rpc端口
    QString rpcServer = "http://localhost:" + rpcPort + "/jsonrpc";//rpc服务器地址
    QString defaultDownloadPath;//默认下载路径
    QString configPath = "";

signals:
    void signalRPCSuccess(QString method, QJsonObject json);
    void signalRPCError(QString method, QString id, int errCode);

public slots:

public:
    static Aria2RPCInterface *Instance();

    static const QString aria2cCmd;//aria2c程序路径
    static const QString basePath;//下载器安装目录
private:
    static Aria2RPCInterface *_pInstance;


public:
    Aria2cBtInfo getBtInfo(QString strTorrentPath);//得到bt文件信息



};

#endif // ARIA2RPCINTERFACE_H
