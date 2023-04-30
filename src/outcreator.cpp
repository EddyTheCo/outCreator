#include"outcreator.hpp"

#include"QJsonDocument"
#include"QJsonObject"

using namespace qiota;

void OutCreator::setAmount(QString amount_m)
{
	amount_=amount_m.toULongLong();
}

void OutCreator::setIssuer(const QJsonValue &issuer)
{
	issuer_=Feature::Issuer(qblocks::Address::from_(issuer));
}
void OutCreator::setSender(const QJsonValue & sender)
{
	sender_=Feature::Sender(qblocks::Address::from_(sender));
}
void OutCreator::setMetadata(QJsonObject metadatajson)
{
	auto metadata=QJsonDocument(metadatajson).toJson();
	metadata_=Feature::Metadata(metadata);

}
void OutCreator::setStateMetadata(QJsonObject metadatajson)
{
	auto metadata=QJsonDocument(metadatajson).toJson();
	state_metadata_=fl_array<quint16>(metadata);
}
void OutCreator::setImmutableMetadata(QJsonObject metadatajson)
{
	auto metadata=QJsonDocument(metadatajson).toJson();
	immutable_metadata_=Feature::Metadata(metadata);
}
void OutCreator::setTag(QString tag)
{
	tag_=Feature::Tag(tag.toUtf8());
}
void OutCreator::setAddressUnlockCondition(const QJsonValue &address)
{
	address_=Unlock_Condition::Address(Address::from_(address));
}
void OutCreator::setStateControllerAddressUnlockCondition(const QJsonValue &address)
{
	state_controller_address_=
		Unlock_Condition::State_Controller_Address(Address::from_(address));
}
void OutCreator::setGovernorAddressUnlockCondition(const QJsonValue &address)
{
	governor_address_=Unlock_Condition::Governor_Address(Address::from_(address));
}
void OutCreator::setImmutableAliasAddressUnlockCondition(const QJsonValue &address)
{
	immutable_alias_address_=Unlock_Condition::Immutable_Alias_Address(Address::from_(address));
}
void OutCreator::setStorageDepositReturnUnlockCondition(const QJsonValue &address, QString amount)
{
	storage_deposit_return_=
		Unlock_Condition::Storage_Deposit_Return(Address::from_(address),amount.toULongLong());
}
void OutCreator::setTimelockUnlockCondition(QDateTime unix_time_m)
{
	timelock_=Unlock_Condition::Timelock(unix_time_m.toSecsSinceEpoch());
}
void OutCreator::setExpirationUnlockCondition(QDateTime unix_time_m, const QJsonValue& address)
{
	expiration_=Unlock_Condition::Expiration(unix_time_m.toSecsSinceEpoch(),Address::from_(address));
}
void OutCreator::fromChain(quint32 index,qiota::AddressBundle bundle)
{
	switch(typ_) {

		case NFT_typ:
			{
				if(bundle.nft_outputs.size()>index)
				{
					out_=bundle.nft_outputs.at(index);
				}
				return;
			}
		case Foundry_typ:
			{
				if(bundle.foundry_outputs.size()>index)
				{
					out_=bundle.foundry_outputs.at(index);
				}
				return;
			}
		case Alias_typ:
			{
				if(bundle.alias_outputs.size()>index)
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
void OutCreator::addNativeToken(const QJsonValue &token)
{
	const auto tokenptr=Native_Token::from_(token);
	native_tokens[tokenptr->token_id()]+=tokenptr->amount();
}
auto OutCreator::getUnlocks()const
{
	pvector<const Unlock_Condition> var;
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
auto OutCreator::getFeatures()const
{
	pvector<const Feature> var;
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
auto OutCreator::getImmutableFeatures()const
{
	pvector<const Feature> var;
	if(typ_!=Basic_typ)
	{
		if(typ_==Alias_typ||typ_==NFT_typ)
		{
			if(issuer_)var.push_back(issuer_);
		}
		if(immutable_metadata_)var.push_back(immutable_metadata_);
		return var;
	}
	return var;
}
auto OutCreator::getNativeTokens()const
{
	pvector<const Native_Token> var;
	for (const auto& v : native_tokens)
	{
		var.push_back(Native_Token::Native(v.first,v.second));
	}
	return var;
}
void OutCreator::setTokenScheme(const QJsonValue& tokenscheme)
{
	token_scheme_=qblocks::Token_Scheme::from_(tokenscheme);
}
void OutCreator::restart()
{
	out_=nullptr;
	foundry_counter_=0;
	state_index_=0;
	serial_number_=0;
	amount_=0;
	typ_=Basic_typ;
	address_=nullptr;
	storage_deposit_return_=nullptr;
	timelock_=nullptr;
	expiration_=nullptr;
	state_controller_address_=nullptr;
	governor_address_=nullptr;
	immutable_alias_address_=nullptr;
	issuer_=nullptr;
	sender_=nullptr;
	metadata_=nullptr;
	immutable_metadata_=nullptr;
	tag_=nullptr;
}
void  OutCreator::init()
{
	qDebug()<<"OutCreator::init";
	switch(typ_) {

		case NFT_typ:
			{
				if(!out_)
				{
					out_=Output::NFT(amount_,getUnlocks(),getNativeTokens(),getImmutableFeatures(),getFeatures());
					qDebug()<<"creating out:"<<out_->get_Json();
				}
				else
				{
					out_->amount_=amount_;
					out_->unlock_conditions_=getUnlocks();
					out_->features_=getFeatures();
					out_->native_tokens_=getNativeTokens();
				}
				break;

			}
		case Foundry_typ:
			{
				if(!out_)
				{
					out_=Output::Foundry(amount_,getUnlocks(),token_scheme_,serial_number_,getNativeTokens(),getImmutableFeatures(),getFeatures());
				}
				else
				{

					out_->amount_=amount_;
					auto output=std::static_pointer_cast<Foundry_Output>(out_);
					output->token_scheme_=token_scheme_;
					out_->features_=getFeatures();
					out_->native_tokens_=getNativeTokens();

				}
				break;

			}
		case Alias_typ:
			{
				if(!out_)
				{
					out_=Output::Alias(amount_,getUnlocks(),state_metadata_,foundry_counter_,state_index_,getNativeTokens(),getImmutableFeatures(),getFeatures());
				}
				else
				{

					out_->amount_=amount_;
					auto output=std::static_pointer_cast<Alias_Output>(out_);
					output->state_index_++;
					output->foundry_counter_=foundry_counter_;
					out_->amount_=amount_;
					out_->unlock_conditions_=getUnlocks();
					out_->features_=getFeatures();
					out_->native_tokens_=getNativeTokens();

				}
				break;

			}
		case Basic_typ:
			{
				out_=Output::Basic(amount_,getUnlocks(),getNativeTokens(),getFeatures());
				break;
			}

	}

	qDebug()<<"creating out:"<<out_->get_Json();
}
