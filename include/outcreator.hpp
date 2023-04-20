#pragma once

#include<QObject>
#include<QString>
#include<QByteArray>
#include<QJsonArray>
#include<QtQml/qqmlregistration.h>
#include"qaddr_bundle.hpp"



using namespace qiota::qblocks;
using namespace qiota;

class OutCreator : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    OutCreator(QObject *parent = nullptr):QObject(parent),typ_(Basic_typ),issuer_(nullptr),sender_(nullptr),metadata_(nullptr),
    immutable_metadata_(nullptr),tag_(nullptr),address_(nullptr),storage_deposit_return_(nullptr),
    timelock_(nullptr),expiration_(nullptr),state_controller_address_(nullptr),governor_address_(nullptr),
    immutable_alias_address_(nullptr),out_(nullptr),foundry_counter_(0),state_index_(0),serial_number_(0),amount_(0){};
    enum types: quint8 { Basic_typ=3,NFT_typ=6,Foundry_typ=5,Alias_typ=4};
    Q_ENUM(types)

    Q_INVOKABLE void fromChain(quint32 index, qiota::AddressBundle bundle);
    Q_INVOKABLE void addNativeToken(QJsonObject token);
    Q_INVOKABLE std::shared_ptr<qblocks::Output>  getOutput(void);


    Q_INVOKABLE void setTyp(OutCreator::types typ_m){typ_=typ_m;};
    Q_INVOKABLE void setAmount(QString amount_m);
    Q_INVOKABLE void setIssuer(QJsonObject issuer);
    Q_INVOKABLE void setSender(QJsonObject sender);
    Q_INVOKABLE void setMetadata(QJsonObject metadata);
    Q_INVOKABLE void setStateMetadata(QJsonObject metadata);
    Q_INVOKABLE void setImmutableMetadata(QJsonObject metadata);
    Q_INVOKABLE void setTag(QString tag);

    Q_INVOKABLE void setAddressUnlockCondition(QJsonObject address);
    Q_INVOKABLE void setStateControllerAddressUnlockCondition(QJsonObject address);
    Q_INVOKABLE void setGovernorAddressUnlockCondition(QJsonObject address);
    Q_INVOKABLE void setImmutableAliasAddressUnlockCondition(QJsonObject address);

    Q_INVOKABLE void setStorageDepositReturnUnlockCondition(QJsonObject address, QString amount);
    Q_INVOKABLE void setTimelockUnlockCondition(QDateTime unix_time_m);
    Q_INVOKABLE void setExpirationUnlockCondition(QDateTime unix_time_m,QJsonObject address);


    Q_INVOKABLE void setSerialNumber(quint32 serial_number_m){serial_number_=serial_number_m;}
    Q_INVOKABLE void setStateIndex(quint32 state_index_m){state_index_=state_index_m;}
    Q_INVOKABLE void setFoundryCounter_(quint32 foundry_counter_m){foundry_counter_=foundry_counter_m;}
    Q_INVOKABLE void setTokenScheme(QJsonObject);




private:
    std::vector<std::shared_ptr<qblocks::Unlock_Condition>> getUnlocks()const;
    std::vector<std::shared_ptr<qblocks::Feature>> getFeatures()const;
    std::vector<std::shared_ptr<qblocks::Feature>> getImmutableFeatures()const;
    std::vector<std::shared_ptr<qblocks::Native_Token>> getNativeTokens()const;
    types typ_;
    quint64 amount_;
    std::shared_ptr<qblocks::Feature> issuer_;
    std::shared_ptr<qblocks::Feature> sender_;
    std::shared_ptr<qblocks::Feature> metadata_;
    std::shared_ptr<qblocks::Feature> immutable_metadata_;
    std::shared_ptr<qblocks::Feature> tag_;

    std::shared_ptr<qblocks::Unlock_Condition> address_;
    std::shared_ptr<qblocks::Unlock_Condition> storage_deposit_return_;
    std::shared_ptr<qblocks::Unlock_Condition> timelock_;
    std::shared_ptr<qblocks::Unlock_Condition> expiration_;
    std::shared_ptr<qblocks::Unlock_Condition> state_controller_address_;
    std::shared_ptr<qblocks::Unlock_Condition> governor_address_;
    std::shared_ptr<qblocks::Unlock_Condition> immutable_alias_address_;

    std::shared_ptr<qblocks::Output> out_;

    quint32 serial_number_,state_index_,foundry_counter_;
    std::shared_ptr<Token_Scheme> token_scheme_;
    std::map<qblocks::c_array,quint256> native_tokens;
    fl_array<quint16> state_metadata_;
};
