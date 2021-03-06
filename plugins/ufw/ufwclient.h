#ifndef UFWCLIENT_H
#define UFWCLIENT_H

#include <QObject>
#include <QString>
#include <QTimer>

#include <KAuth>

#include "profile.h"
#include "rulelistmodel.h"
#include "loglistmodel.h"

class UfwClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY isBusyChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString defaultIncomingPolicy READ defaultIncomingPolicy WRITE setDefaultIncomingPolicy NOTIFY defaultIncomingPolicyChanged)
    Q_PROPERTY(QString defaultOutgoingPolicy READ defaultOutgoingPolicy WRITE setDefaultOutgoingPolicy NOTIFY defaultOutgoingPolicyChanged)
    Q_PROPERTY(bool logsAutoRefresh READ logsAutoRefresh WRITE setLogsAutoRefresh NOTIFY logsAutoRefreshChanged)
public:
    explicit UfwClient(QObject *parent = nullptr);

    Q_INVOKABLE void refresh();
    Q_INVOKABLE RuleListModel* rules() const;
    Q_INVOKABLE RuleWrapper* getRule(int index);
    Q_INVOKABLE void addRule(RuleWrapper * rule);
    Q_INVOKABLE void removeRule(int index);
    Q_INVOKABLE void updateRule(RuleWrapper * rule);
    Q_INVOKABLE void moveRule(int from, int to);

    Q_INVOKABLE static QStringList getKnownProtocols();
    Q_INVOKABLE static QStringList getKnownInterfaces();

    bool enabled() const;
    bool isBusy() const;
    QString status() const;
    QString defaultIncomingPolicy() const;
    QString defaultOutgoingPolicy() const;

    Q_INVOKABLE LogListModel* logs();
    bool logsAutoRefresh() const;

signals:
    void isBusyChanged(const bool isBusy);
    void enabledChanged(const bool enabled);
    void statusChanged(const QString &status);

    void defaultIncomingPolicyChanged(QString defaultIncomingPolicy);
    void defaultOutgoingPolicyChanged(QString defaultOutgoingPolicy);


    void logsAutoRefreshChanged(bool logsAutoRefresh);

public slots:
    void setEnabled(const bool &enabled);
    void queryStatus(bool readDefaults=true, bool listProfiles=true);
    void setDefaultIncomingPolicy(QString defaultIncomingPolicy);
    void setDefaultOutgoingPolicy(QString defaultOutgoingPolicy);

    void setLogsAutoRefresh(bool logsAutoRefresh);

protected slots:
        void refreshLogs();

protected:
    void setStatus(const QString &status);
    void setBusy(const bool &busy);
    void setProfile(UFW::Profile profile);
    KAuth::Action buildQueryAction(const QVariantMap &arguments);
    KAuth::Action buildModifyAction(const QVariantMap &arguments);

private:
    QString m_status;
    QStringList         m_rawLogs;
    bool                m_isBusy;
    UFW::Profile        m_currentProfile;
    RuleListModel*      m_rulesModel;
    LogListModel*       m_logs;
    QTimer              m_logsRefreshTimer;
    //    UFW::Blocker       *blocker;
    bool m_logsAutoRefresh;
};

#endif // UFWCLIENT_H
