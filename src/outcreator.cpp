#include"outcreator.hpp"

#include"QJsonDocument"
#include"QJsonObject"

using namespace qiota;

void OutCreator::setAmount(QString amount_m)
{
    amount_=amount_m.toULongLong();
}
void OutCreator::setIssuer(QJsonObject issuer)
{
    issuer_=std::shared_ptr<qblocks::Feature>(new qblocks::Issuer_Feature(qblocks::Address::from_(issuer)));
}
void OutCreator::setSender(QJsonObject sender)
{
    sender_=std::shared_ptr<qblocks::Feature>(new qblocks::Sender_Feature(qblocks::Address::from_(sender)));
}
void OutCreator::setMetadata(QJsonObject metadatajson)
{
    auto metadata=QJsonDocument(metadatajson).toJson(QJsonDocument::Indented);
    auto metadata_=std::shared_ptr<qblocks::Feature>(new Metadata_Feature(fl_array<quint16>(metadata)));
}
void OutCreator::setStateMetadata(QJsonObject metadatajson)
{
    auto metadata=QJsonDocument(metadatajson).toJson(QJsonDocument::Indented);
    auto state_metadata_=fl_array<quint16>(metadata);
}
void OutCreator::setImmutableMetadata(QJsonObject metadatajson)
{
    auto metadata=QJsonDocument(metadatajson).toJson(QJsonDocument::Indented);
    auto immutable_metadata_=std::shared_ptr<qblocks::Feature>(new Metadata_Feature(fl_array<quint16>(metadata)));
}
void OutCreator::setTag(QString tag)
{
    tag_=std::shared_ptr<qblocks::Feature>(new Tag_Feature(fl_array<quint8>(tag.toUtf8())));
}
void OutCreator::setAddressUnlockCondition(QJsonObject address)
{
    address_=std::shared_ptr<qblocks::Unlock_Condition>(new Address_Unlock_Condition(qblocks::Address::from_(address)));
}
void OutCreator::setStateControllerAddressUnlockCondition(QJsonObject address)
{
    state_controller_address_=std::shared_ptr<qblocks::Unlock_Condition>(new State_Controller_Address_Unlock_Condition(qblocks::Address::from_(address)));
}
void OutCreator::setGovernorAddressUnlockCondition(QJsonObject address)
{
    governor_address_=std::shared_ptr<qblocks::Unlock_Condition>(new Governor_Address_Unlock_Condition(qblocks::Address::from_(address)));
}
void OutCreator::setImmutableAliasAddressUnlockCondition(QJsonObject address)
{
    immutable_alias_address_=std::shared_ptr<qblocks::Unlock_Condition>(new Immutable_Alias_Address_Unlock_Condition(qblocks::Address::from_(address)));
}
void OutCreator::setStorageDepositReturnUnlockCondition(QJsonObject address, QString amount)
{
    storage_deposit_return_=std::shared_ptr<qblocks::Unlock_Condition>
            (new Storage_Deposit_Return_Unlock_Condition(qblocks::Address::from_(address),amount.toULongLong()));
}
void OutCreator::setTimelockUnlockCondition(QDateTime unix_time_m)
{
    timelock_=std::shared_ptr<qblocks::Unlock_Condition>
            (new Timelock_Unlock_Condition(unix_time_m.toSecsSinceEpoch()));
}
void OutCreator::setExpirationUnlockCondition(QDateTime unix_time_m, QJsonObject address)
{
    expiration_=std::shared_ptr<qblocks::Unlock_Condition>
            (new Expiration_Unlock_Condition(unix_time_m.toSecsSinceEpoch(),qblocks::Address::from_(address)));
}
void OutCreator::fromChain(quint32 index,qiota::AddressBundle bundle)
{
    switch(typ_) {

    case NFT_typ:
    {
        if(bundle.nft_outputs.size()<index)
        {
            out_=bundle.nft_outputs.at(index);
        }
        return;
    }
    case Foundry_typ:
    {
        if(bundle.foundry_outputs.size()<index)
        {
            out_=bundle.foundry_outputs.at(index);
        }
        return;
    }
    case Alias_typ:
    {
        if(bundle.alias_outputs.size()<index)
        {
            out_=bundle.alias_outputs.at(index);
        }
        return;
    }
    case Basic_typ:
    {
        return;
    }

    }
}
void OutCreator::addNativeToken(QJsonObject token)
{
    const auto tokenptr=qblocks::Native_Token::from_(token);
    native_tokens[tokenptr->token_id()]+=tokenptr->amount();
}
std::vector<std::shared_ptr<qblocks::Unlock_Condition>> OutCreator::getUnlocks()const
{
    std::vector<std::shared_ptr<qblocks::Unlock_Condition>> var;
    if(typ_==Alias_typ)
    {
        var.push_back(state_controller_address_);
        var.push_back(governor_address_);
        return var;
    }
    if(typ_==Foundry_typ)
    {
        var.push_back(immutable_alias_address_);
        return var;
    }
    var.push_back(address_);
    if(storage_deposit_return_)var.push_back(storage_deposit_return_);
    if(timelock_)var.push_back(timelock_);
    if(expiration_)var.push_back(expiration_);
    return var;
}
std::vector<std::shared_ptr<qblocks::Feature>> OutCreator::getFeatures()const
{
    std::vector<std::shared_ptr<qblocks::Feature>> var;
    if(typ_==Alias_typ)
    {
        if(sender_)var.push_back(sender_);
        if(metadata_)var.push_back(metadata_);
        return var;
    }
    if(typ_==Foundry_typ)
    {
        if(metadata_)var.push_back(metadata_);
        return var;
    }

    if(sender_)var.push_back(sender_);
    if(metadata_)var.push_back(metadata_);
    if(tag_)var.push_back(tag_);
    return var;
}
std::vector<std::shared_ptr<qblocks::Feature>> OutCreator::getImmutableFeatures()const
{
    std::vector<std::shared_ptr<qblocks::Feature>> var;
    if(typ_==Alias_typ)
    {
        if(issuer_)var.push_back(issuer_);
        if(immutable_metadata_)var.push_back(immutable_metadata_);
        return var;
    }
    if(typ_!=Basic_typ)
    {
        if(immutable_metadata_)var.push_back(metadata_);
        return var;
    }
    return var;
}
std::vector<std::shared_ptr<qblocks::Native_Token>> OutCreator::getNativeTokens()const
{
    std::vector<std::shared_ptr<qblocks::Native_Token>> var;
    for (const auto& v : native_tokens)
    {
        var.push_back(std::shared_ptr<qblocks::Native_Token>(new qblocks::Native_Token(v.first,v.second)));
    }
    return var;
}
void OutCreator::setTokenScheme(QJsonObject tokenscheme)
{
    token_scheme_=qblocks::Token_Scheme::from_(tokenscheme);
}
std::shared_ptr<qblocks::Output>  OutCreator::getOutput(void)
{
    switch(typ_) {

    case NFT_typ:
    {
        if(!out_)
        {
            out_=std::shared_ptr<qblocks::Output>(new NFT_Output(amount_,getUnlocks(),getFeatures(),getNativeTokens(),getImmutableFeatures()));
        }
        else
        {
            out_->amount_=amount_;
            out_->unlock_conditions_=getUnlocks();
            out_->features_=getFeatures();
            out_->native_tokens_=getNativeTokens();
        }
        return out_;
    }
    case Foundry_typ:
    {
        if(!out_)
        {
            out_=std::shared_ptr<qblocks::Output>(new Foundry_Output(amount_,getUnlocks(),token_scheme_,serial_number_,getFeatures(),getNativeTokens(),getImmutableFeatures()));
        }
        else
        {
            out_->amount_=amount_;
            if(token_scheme_)
            {
                auto output=std::dynamic_pointer_cast<qblocks::Foundry_Output>(out_);
                output->token_scheme_=token_scheme_;
            }
            out_->features_=getFeatures();
            out_->native_tokens_=getNativeTokens();
        }
        return out_;
    }
    case Alias_typ:
    {
        if(!out_)
        {
            out_=std::shared_ptr<qblocks::Output>(new Alias_Output(amount_,getUnlocks(),state_index_,foundry_counter_,state_metadata_,getFeatures(),getNativeTokens(),getImmutableFeatures()));
        }
        else
        {
            out_->amount_=amount_;
            auto output=std::dynamic_pointer_cast<qblocks::Alias_Output>(out_);
            output->state_index_++;
            output->foundry_counter_=foundry_counter_;
            out_->amount_=amount_;
            out_->unlock_conditions_=getUnlocks();
            out_->features_=getFeatures();
            out_->native_tokens_=getNativeTokens();
        }
        return out_;
    }
    case Basic_typ:
    {
        return out_=std::shared_ptr<qblocks::Output>(new Basic_Output(amount_,getUnlocks(),getFeatures(),getNativeTokens()));
    }

    }
    return out_;
}
