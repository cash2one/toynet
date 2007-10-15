/**
* Copyright (c) 2004 Mega Enterprise Co., Ltd.
*
* @author ±èº´¼ö <kcando@megaking.co.kr>
*/
// PoporuWdrUtil.h : Arcade WDR utilities
//
#if !defined(INCLUDED_POPORUWDRUTIL)
#define INCLUDED_POPORUWDRUTIL

#include "../ArcadeGameScoreInfo.h"
#include "../ArcadeServerInfo.h"
#include "../im/Messenger.h"
#include <skynet/detail/WdrUtil.h>
#include <skynet/arcade/ServerType.h>
#include <string>
#include <vector>

namespace skynet
{

// = time_t

inline bool readWdrTime(Wdr::InputWdr& wdr, time_t& atime)
{
    Wdr::Long value;
    if (! wdr.read(value))
        return false;
    atime = static_cast<time_t>(value);
    return true;
}


inline bool writeWdrTime(Wdr::OutputWdr& wdr, time_t atime)
{
    return wdr.write(static_cast<Wdr::Long>(atime));
}


inline size_t getWdrTimeSize()
{
    return Wdr::sizeLong;
}


inline bool readWdrChannelType(Wdr::InputWdr& wdr, ChannelType& channelType)
{
    Wdr::Octet value;
    if (! wdr.read(value))
        return false;
    channelType = static_cast<ChannelType>(value);
    return isValidChannelType(channelType);
}


inline bool writeWdrChannelType(Wdr::OutputWdr& wdr, ChannelType channelType)
{
    return wdr.write(static_cast<Wdr::Octet>(channelType));
}


inline size_t getWdrChannelTypeSize()
{
    return Wdr::sizeOctet;
}


//bool VecString
bool WARNET_API readWdrVecString(Wdr::InputWdr& wdr, 
    std::vector<std::string>& vecString, size_t size);
bool WARNET_API writeWdrVecString(Wdr::OutputWdr& wdr,
    const std::vector<std::string>& vecString);
size_t WARNET_API getWdrVecStringSize(const std::vector<std::string> & vecString);


//=AuthenticMode
bool WARNET_API readWdrAuthenticMode(Wdr::InputWdr& wdr, 
    mode::AuthenticMode& authenticMode);
bool WARNET_API writeWdrAuthenticMode(Wdr::OutputWdr& wdr,
    const mode::AuthenticMode& authenticMode);
size_t WARNET_API getWdrAuthenticModeSize();


//=AuthenticImfo
bool WARNET_API readWdrAuthenticImpo(Wdr::InputWdr& wdr, 
    AuthenticImpo&  authenticImpo);
bool WARNET_API writeWdrAuthenticImpo(Wdr::OutputWdr& wdr,
    const AuthenticImpo& authenticImpo);
size_t WARNET_API getWdrAuthenticImpo(const AuthenticImpo& authenticImpo);


//=ToolUseGrade
bool WARNET_API readWdrToolUseGrade(Wdr::InputWdr& wdr, 
    ToolUseGrade& toolUseGrade);
bool WARNET_API writeWdrToolUseGrade(Wdr::OutputWdr& wdr,
    const ToolUseGrade& toolUseGrade);
size_t WARNET_API getWdrToolUseGradeSize();


//=ServerType
bool WARNET_API readWdrServerType(Wdr::InputWdr& wdr, ServerType& serverType);
bool WARNET_API writeWdrServerType(Wdr::OutputWdr& wdr,
    const ServerType& serverType);
size_t WARNET_API getWdrServerTypeSize();


//=ServerListInfos
bool WARNET_API readWdrServerListInfos(Wdr::InputWdr& wdr,
    ServerListInfos& serverListInfos);
bool WARNET_API writeWdrServerListInfos(Wdr::OutputWdr& wdr,
    const ServerListInfos& serverListInfos);
size_t WARNET_API getWdrServerListInfosSize(const ServerListInfos& serverListInfos);


//=ServerListInfo
bool WARNET_API readWdrServerListInfo(Wdr::InputWdr& wdr,
    ServerListInfo& serverListInfo);
bool WARNET_API writeWdrServerListInfo(Wdr::OutputWdr& wdr,
    const ServerListInfo& serverListInfo);
size_t WARNET_API getWdrServerListInfo(const ServerListInfo& serverListInfo);


//=ServerIds
bool WARNET_API readWdrServerIds(Wdr::InputWdr& wdr, ServerIds& serverIds);
bool WARNET_API writeWdrServerIds(Wdr::OutputWdr& wdr, const ServerIds& serverIds);
size_t WARNET_API getWdrServerIdsSize(const ServerIds& serverIds);


//GameChannelInfo
bool WARNET_API readWdrGameChannelInfo(Wdr::InputWdr& wdr, 
    GameChannelInfo& gameChannelInfo);
bool WARNET_API writeWdrGameChannelInfo(Wdr::OutputWdr& wdr,
    const GameChannelInfo& gameChannelInfo);
size_t WARNET_API  getWdrGameChannelInfoSize();


//GameChannelInfoMap
bool WARNET_API readWdrGameChannelInfoMap(Wdr::InputWdr& wdr, 
    GameChannelInfoMap& gameChannelInfoMap);
bool WARNET_API writeWdrGameChannelInfoMap(Wdr::OutputWdr& wdr,
    const GameChannelInfoMap& gameChannelInfoMap);
size_t WARNET_API  getWdrGameChannelInfoMapSize(
    const GameChannelInfoMap& gameChannelInfoMap);


// = serverState
bool WARNET_API readWdrServerState(Wdr::InputWdr& wdr,
    ServerState& serverState);
bool WARNET_API writeWdrServerState(Wdr::OutputWdr& wdr,
    const ServerState& serverState);
size_t  WARNET_API getWdrServerStateSize();


// = serverStateMap
bool WARNET_API readWdrServerStateMap(Wdr::InputWdr& wdr,
    ServerStateMap& serverStateMap);
bool WARNET_API writeWdrServerStateMap(Wdr::OutputWdr& wdr,
    const ServerStateMap& serverStateMap);
size_t  WARNET_API getWdrServerStateMapSize(
    const ServerStateMap& serverStateMap);


// = ChannelId
bool WARNET_API readWdrChannelId(Wdr::InputWdr& wdr, ChannelId& channelId);
bool WARNET_API writeWdrChannelId(Wdr::OutputWdr& wdr,
    ChannelId channelId);
size_t  WARNET_API getWdrChannelIdSize();


// = ChannelIds
bool WARNET_API readWdrChannelIds(Wdr::InputWdr& wdr, ChannelIds& channelIds);
bool WARNET_API writeWdrChannelIds(Wdr::OutputWdr& wdr, ChannelIds channelIds);
size_t  WARNET_API getWdrChannelIdsSize(const ChannelIds& channelIds);


// = BasicItemInfo

bool WARNET_API readWdrBasicItemInfo(Wdr::InputWdr& wdr, BasicItemInfo& item);
bool WARNET_API writeWdrBasicItemInfo(Wdr::OutputWdr& wdr,
    const BasicItemInfo& item);
size_t WARNET_API getWdrBasicItemInfoSize();

// = ItemInfo

bool WARNET_API readWdrItemInfo(Wdr::InputWdr& wdr, ItemInfo& item);
bool WARNET_API writeWdrItemInfo(Wdr::OutputWdr& wdr, const ItemInfo& item);
size_t WARNET_API getWdrItemInfoSize();

// = ItemInfos

bool WARNET_API readWdrItemInfos(Wdr::InputWdr& wdr, ItemInfos& items);
bool WARNET_API writeWdrItemInfos(Wdr::OutputWdr& wdr, const ItemInfos& items);
size_t WARNET_API getWdrItemInfosSize(const ItemInfos& items);

// = SlotItemInfo

bool WARNET_API readWdrSlotItemInfo(Wdr::InputWdr& wdr, SlotItemInfo& item);
bool WARNET_API writeWdrSlotItemInfo(Wdr::OutputWdr& wdr,
    const SlotItemInfo& item);
size_t WARNET_API getWdrSlotItemInfoSize();

// = SlotItemInfos

bool WARNET_API readWdrSlotItemInfos(Wdr::InputWdr& wdr,
    SlotItemInfos& items);
bool WARNET_API writeWdrSlotItemInfos(Wdr::OutputWdr& wdr,
    const SlotItemInfos& items);
size_t WARNET_API getWdrSlotItemInfosSize(const SlotItemInfos& items);

// = CardProperty

bool WARNET_API readWdrCardProperty(Wdr::InputWdr& wdr,
    CardProperty& property);
bool WARNET_API writeWdrCardProperty(Wdr::OutputWdr& wdr,
    const CardProperty& property);
size_t WARNET_API getWdrCardPropertySize(const CardProperty& property);

// = CharacterCard

bool WARNET_API readWdrCharacterCard(Wdr::InputWdr& wdr,
    CharacterCard& card);
bool WARNET_API writeWdrCharacterCard(Wdr::OutputWdr& wdr,
    const CharacterCard& card);
size_t WARNET_API getWdrCharacterCardSize(const CharacterCard& card);

// = CharacterCards

bool WARNET_API readWdrCharacterCards(Wdr::InputWdr& wdr,
    CharacterCards& cards);
bool WARNET_API writeWdrCharacterCards(Wdr::OutputWdr& wdr,
    const CharacterCards& cards);
size_t WARNET_API getWdrCharacterCardsSize(const CharacterCards& cards);

// = CoreUserInfo

bool WARNET_API readWdrCoreUserInfo(Wdr::InputWdr& wdr,
    CoreUserInfo& ui);
bool WARNET_API writeWdrCoreUserInfo(Wdr::OutputWdr& wdr,
    const CoreUserInfo& ui);
size_t WARNET_API getWdrCoreUserInfoSize(const CoreUserInfo& ui);

// = CommonUserInfo

bool WARNET_API readWdrCommonUserInfo(Wdr::InputWdr& wdr,
    CommonUserInfo& ui);
bool WARNET_API writeWdrCommonUserInfo(Wdr::OutputWdr& wdr,
    const CommonUserInfo& ui);
size_t WARNET_API getWdrCommonUserInfoSize(const CommonUserInfo& ui);

// = FullUserInfo

bool WARNET_API readWdrFullUserInfo(Wdr::InputWdr& wdr,
    FullUserInfo& ui);
bool WARNET_API writeWdrFullUserInfo(Wdr::OutputWdr& wdr,
    const FullUserInfo& ui);
size_t WARNET_API getWdrFullUserInfoSize(const FullUserInfo& ui);

// = UserInfo

bool WARNET_API readWdrUserInfo(Wdr::InputWdr& wdr,
    UserInfo& ui);
bool WARNET_API writeWdrUserInfo(Wdr::OutputWdr& wdr,
    const UserInfo& ui);
size_t WARNET_API getWdrUserInfoSize(const UserInfo& ui);

// = MatchGameRecord

bool WARNET_API readWdrMatchGameRecord(Wdr::InputWdr& wdr,
    MatchGameRecord& gameRecord);
bool WARNET_API writeWdrMatchGameRecord(Wdr::OutputWdr& wdr,
    const MatchGameRecord& gameRecord);
size_t WARNET_API getWdrMatchGameRecordSize();

// = CooperativeGameRecord

bool WARNET_API readWdrCooperativeGameRecord(Wdr::InputWdr& wdr,
    CooperativeGameRecord& gameRecord);
bool WARNET_API writeWdrCooperativeGameRecord(Wdr::OutputWdr& wdr,
    const CooperativeGameRecord& gameRecord);
size_t WARNET_API getWdrCooperativeGameRecordSize();

// = GameScore

bool WARNET_API readWdrGameScore(Wdr::InputWdr& wdr, GameScore& gameScore);
bool WARNET_API writeWdrGameScore(Wdr::OutputWdr& wdr,
    const GameScore& gameScore);
size_t WARNET_API getWdrGameScoreSize(const GameScore& gameScore);

// = GameScores

bool WARNET_API readWdrGameScores(Wdr::InputWdr& wdr,
    GameScores& gameScores);
bool WARNET_API writeWdrGameScores(Wdr::OutputWdr& wdr,
    const GameScores& gameScores);
size_t WARNET_API getWdrGameScoresSize(const GameScores& gameScores);

// = GameScoreByGameTypes

bool WARNET_API readWdrGameScoreByGameTypes(Wdr::InputWdr& wdr,
    GameScoreByGameTypes& gameScores);
bool WARNET_API writeWdrGameScoreByGameTypes(Wdr::OutputWdr& wdr,
    const GameScoreByGameTypes& gameScores);
size_t WARNET_API getWdrGameScoreByGameTypesSize(
    const GameScoreByGameTypes& gameScores);

// = MatchGameRecordByGameModes

bool WARNET_API readWdrMatchGameRecordByGameModes(Wdr::InputWdr& wdr,
    MatchGameRecordByGameModes& gameRecords);
bool WARNET_API writeWdrMatchGameRecordByGameModes(Wdr::OutputWdr& wdr,
    const MatchGameRecordByGameModes& gameRecords);
size_t WARNET_API getWdrMatchGameRecordByGameModesSize(
    const MatchGameRecordByGameModes& gameRecords);

// = DetailedUserInfo

bool WARNET_API readWdrDetailedUserInfo(Wdr::InputWdr& wdr,
    DetailedUserInfo& ui);
bool WARNET_API writeWdrDetailedUserInfo(Wdr::OutputWdr& wdr,
    const DetailedUserInfo& ui);
size_t WARNET_API getWdrDetailedUserInfoSize(const DetailedUserInfo& ui);

// = AddressPair

bool WARNET_API readWdrAddressPair(Wdr::InputWdr& wdr, AddressPair& pair);
bool WARNET_API writeWdrAddressPair(Wdr::OutputWdr& wdr,
    const AddressPair& pair);
size_t WARNET_API getWdrAddressPairSize(const AddressPair& pair);

// = AddressPairs

bool WARNET_API readWdrAddressPairs(Wdr::InputWdr& wdr,
    AddressPairs& pairs);
bool WARNET_API writeWdrAddressPairs(Wdr::OutputWdr& wdr,
    const AddressPairs& pairs);
size_t WARNET_API getWdrAddressPairsSize(const AddressPairs& pairs);

// = Channel

bool WARNET_API readWdrChannel(Wdr::InputWdr& wdr, Channel& channel);
bool WARNET_API writeWdrChannel(Wdr::OutputWdr& wdr,
    const Channel& channel);
size_t WARNET_API getWdrChannelSize(const Channel& channel);

// = Channels

bool WARNET_API readWdrChannels(Wdr::InputWdr& wdr, Channels& channels);
bool WARNET_API writeWdrChannels(Wdr::OutputWdr& wdr,
    const Channels& channels);
size_t WARNET_API getWdrChannelsSize(const Channels& channels);

// = CoinStorage

bool WARNET_API readWdrCoinStorage(Wdr::InputWdr& wdr, CoinStorage& storage);
bool WARNET_API readWdrPointStorage(Wdr::InputWdr& wdr, PointStorage& storage);

bool WARNET_API writeWdrCoinStorage(Wdr::OutputWdr& wdr,
    const CoinStorage& storage);

bool WARNET_API writeWdrPointStorage(Wdr::OutputWdr& wdr,
    const PointStorage& storage);

size_t WARNET_API getWdrCoinStorageSize();
size_t WARNET_API getWdrPointStorageSize();

// = ChannelWaiter

bool WARNET_API readWdrChannelWaiter(Wdr::InputWdr& wdr,
    ChannelWaiter& waiter);
bool WARNET_API writeWdrChannelWaiter(Wdr::OutputWdr& wdr,
    const ChannelWaiter& waiter);
size_t WARNET_API getWdrChannelWaiterSize(const ChannelWaiter& waiter);

// = ChannelWaiters

bool WARNET_API readWdrChannelWaiters(Wdr::InputWdr& wdr,
    ChannelWaiters& waiters);
bool WARNET_API writeWdrChannelWaiters(Wdr::OutputWdr& wdr,
    const ChannelWaiters& waiters);
size_t WARNET_API getWdrChannelWaitersSize(const ChannelWaiters& waiters);

// = Gamer

bool WARNET_API readWdrGamer(Wdr::InputWdr& wdr, Gamer& gamer);
bool WARNET_API writeWdrGamer(Wdr::OutputWdr& wdr, const Gamer& gamer);
size_t WARNET_API getWdrGamerSize(const Gamer& gamer);

// = Gamers

bool WARNET_API readWdrGamers(Wdr::InputWdr& wdr, Gamers& gamers);
bool WARNET_API writeWdrGamers(Wdr::OutputWdr& wdr, const Gamers& gamers);
size_t WARNET_API getWdrGamersSize(const Gamers& gamers);

// = GameRoomInfo

bool WARNET_API readWdrGameRoomInfo(Wdr::InputWdr& wdr,
    GameRoomInfo& roomInfo);
bool WARNET_API writeWdrGameRoomInfo(Wdr::OutputWdr& wdr,
    const GameRoomInfo& roomInfo);
size_t WARNET_API getWdrGameRoomInfoSize(const GameRoomInfo& roomInfo);

// = GameRoomInfos

bool WARNET_API readWdrGameRoomInfos(Wdr::InputWdr& wdr,
    GameRoomInfos& roomInfos);
bool WARNET_API writeWdrGameRoomInfos(Wdr::OutputWdr& wdr,
    const GameRoomInfos& roomInfos);
size_t WARNET_API getWdrGameRoomInfosSize(const GameRoomInfos& roomInfos);

// = GameRoomWaiter

bool WARNET_API readWdrGameRoomWaiter(Wdr::InputWdr& wdr,
    GameRoomWaiter& grw);
bool WARNET_API writeWdrGameRoomWaiter(Wdr::OutputWdr& wdr,
    const GameRoomWaiter& grw);
size_t WARNET_API getWdrGameRoomWaiterSize(const GameRoomWaiter& grw);

// = GameRoomWaiters

bool WARNET_API readWdrGameRoomWaiters(Wdr::InputWdr& wdr,
    GameRoomWaiters& grws);
bool WARNET_API writeWdrGameRoomWaiters(Wdr::OutputWdr& wdr,
    const GameRoomWaiters& grws);
size_t WARNET_API getWdrGameRoomWaitersSize(const GameRoomWaiters& grws);

// = PlayerAddressInfo

bool WARNET_API readWdrPlayerAddressInfo(Wdr::InputWdr& wdr,
    PlayerAddressInfo& addressInfo);
bool WARNET_API writeWdrPlayerAddressInfo(Wdr::OutputWdr& wdr,
    const PlayerAddressInfo& addressInfo);
size_t WARNET_API getWdrPlayerAddressInfoSize(
    const PlayerAddressInfo& addressInfo);






// = PlayerAddressInfos

bool WARNET_API readWdrPlayerAddressInfos(Wdr::InputWdr& wdr,
    PlayerAddressInfos& addressInfos);
bool WARNET_API writeWdrPlayerAddressInfos(Wdr::OutputWdr& wdr,
    const PlayerAddressInfos& addressInfos);
size_t WARNET_API getWdrPlayerAddressInfosSize(
    const PlayerAddressInfos& addressInfos);

// = GameResult

bool WARNET_API readWdrGameResult(Wdr::InputWdr& wdr, GameResult& result);
bool WARNET_API writeWdrGameResult(Wdr::OutputWdr& wdr,
    const GameResult& result);
size_t WARNET_API getWdrGameResultSize(const GameResult& result);

// = GameServerInfoMap

bool WARNET_API readWdrGameServerInfoMap(Wdr::InputWdr& wdr,
    GameServerInfoMap& gameServerInfoMap);
bool WARNET_API writeWdrGameServerInfoMap(Wdr::OutputWdr& wdr,
    const GameServerInfoMap& gameServerInfoMap);
size_t WARNET_API getWdrGameServerInfoMapSize(const GameServerInfoMap&
    gameServerInfoMap);

// = SlotIndex

inline bool readWdrSlotIndex(Wdr::InputWdr& wdr, SlotIndex& slotIndex) {
    Wdr::Octet index;
    if (! wdr.read(index))
        return false;
    slotIndex = static_cast<SlotIndex>(index);
    return isValidSlotIndex(slotIndex);
}

inline bool writeWdrSlotIndex(Wdr::OutputWdr& wdr, SlotIndex slotIndex) {
    return wdr.write(static_cast<Wdr::Octet>(slotIndex));
}

inline size_t getWdrSlotIndexSize() {
    return Wdr::sizeOctet;
}

// = SlotState

inline bool readWdrSlotState(Wdr::InputWdr& wdr, SlotState& slotState) {
    Wdr::Octet state;
    if (! wdr.read(state))
        return false;
    slotState = static_cast<SlotState>(state);
    return isValidSlotState(slotState);
}

inline bool writeWdrSlotState(Wdr::OutputWdr& wdr, SlotState slotState) {
    return wdr.write(static_cast<Wdr::Octet>(slotState));
}

inline size_t getWdrSlotStateSize() {
    return Wdr::sizeOctet;
}

// = ReadyFlag

inline bool readWdrReadyFlag(Wdr::InputWdr& wdr, ReadyFlag& readyFlag) {
    Wdr::Octet flag;
    if (! wdr.read(flag))
        return false;
    readyFlag = static_cast<ReadyFlag>(flag);
    return isValidReadyFlag(readyFlag);
}

inline bool writeWdrReadyFlag(Wdr::OutputWdr& wdr, ReadyFlag readyFlag) {
    return wdr.write(static_cast<Wdr::Octet>(readyFlag));
}

inline size_t getWdrReadyFlagSize() {
    return Wdr::sizeOctet;
}

// = SlotInfo

inline bool readWdrSlotInfo(Wdr::InputWdr& wdr, SlotInfo& slotInfo) {
    return readWdrSlotState(wdr, slotInfo.state_) &&
        readWdrReadyFlag(wdr, slotInfo.flag_);
}

inline bool writeWdrSlotInfo(Wdr::OutputWdr& wdr, SlotInfo slotInfo) {
    return writeWdrSlotState(wdr, slotInfo.state_) &&
        writeWdrReadyFlag(wdr, slotInfo.flag_);
}

inline size_t getWdrSlotInfoSize() {
    return getWdrSlotStateSize() + getWdrReadyFlagSize();
}

// = SlotInfos

bool WARNET_API readWdrSlotInfos(Wdr::InputWdr& wdr,
    SlotInfos& slotInfos);
bool WARNET_API writeWdrSlotInfos(Wdr::OutputWdr& wdr,
    const SlotInfos& slotInfos);
size_t WARNET_API getWdrSlotInfosSize(const SlotInfos& slotInfos);

// = DetailedSlotInfos

bool WARNET_API readWdrDetailedSlotInfos(Wdr::InputWdr& wdr,
    DetailedSlotInfos& detailedSlotInfos);
bool WARNET_API writeWdrDetailedSlotInfos(Wdr::OutputWdr& wdr,
    const DetailedSlotInfos& detailedSlotInfos);
size_t WARNET_API getWdrDetailedSlotInfosSize(
    const DetailedSlotInfos& detailedSlotInfos);

// = DetailedGameRoomInfo

bool WARNET_API readWdrDetailedGameRoomInfo(Wdr::InputWdr& wdr,
    DetailedGameRoomInfo& roomInfo);
bool WARNET_API writeWdrDetailedGameRoomInfo(Wdr::OutputWdr& wdr,
    const DetailedGameRoomInfo& roomInfo);
size_t WARNET_API getWdrDetailedGameRoomInfoSize(
    const DetailedGameRoomInfo& roomInfo);

// = GameRoundInfo

bool WARNET_API readWdrGameRoundInfo(Wdr::InputWdr& wdr,
    GameRoundInfo& gameRoundInfo);
bool WARNET_API writeWdrGameRoundInfo(Wdr::OutputWdr& wdr,
    const GameRoundInfo& gameRoundInfo);
size_t WARNET_API getWdrGameRoundInfoSize(
    const GameRoundInfo& gameRoundInfo);

// = GameRoundInfos

bool WARNET_API readWdrGameRoundInfos(Wdr::InputWdr& wdr,
    GameRoundInfos& gameRoundInfos);
bool WARNET_API writeWdrGameRoundInfos(Wdr::OutputWdr& wdr,
    const GameRoundInfos& gameRoundInfos);
size_t WARNET_API getWdrGameRoundInfosSize(
    const GameRoundInfos& gameRoundInfos);

// = ChannelGroups

bool WARNET_API readWdrChannelGroups(Wdr::InputWdr& wdr,
    ChannelGroups& channelGroups);
bool WARNET_API writeWdrChannelGroups(Wdr::OutputWdr& wdr,
    const ChannelGroups& channelGroups);
size_t WARNET_API getWdrChannelGroupsSize(
    const ChannelGroups& channelGroups);

// = LoginServerInfo

inline bool readWdrLoginServerInfo(Wdr::InputWdr& wdr,
        LoginServerInfo& info) {
    return readWdrServerState(wdr,info.serverState_) &&
        readWdrChannelGroups(wdr, info.channelGroups_);
}
inline bool writeWdrLoginServerInfo(Wdr::OutputWdr& wdr,
        const LoginServerInfo& info) {
    return writeWdrServerState(wdr,info.serverState_) &&
        writeWdrChannelGroups(wdr, info.channelGroups_);
}
inline size_t getWdrLoginServerInfoSize(const LoginServerInfo& info) {
    return getWdrChannelGroupsSize(info.channelGroups_) +
        getWdrServerStateSize();
}

// = GameServerInfo

bool WARNET_API readWdrGameServerInfo(Wdr::InputWdr& wdr,
    GameServerInfo& info);
bool WARNET_API writeWdrGameServerInfo(Wdr::OutputWdr& wdr,
    const GameServerInfo& info);
size_t WARNET_API getWdrGameServerInfoSize(const GameServerInfo& info);

// = RelayServerInfo

inline bool readWdrRelayServerInfo(Wdr::InputWdr& wdr,
        RelayServerInfo& info) {
    return readWdrServerState(wdr,info.serverState_) &&
        wdr.read(info.totalRelayRoomCount_);
}

inline bool writeWdrRelayServerInfo(Wdr::OutputWdr& wdr,
        const RelayServerInfo& info) {
    return writeWdrServerState(wdr,info.serverState_) &&
        wdr.write(info.totalRelayRoomCount_);
}

inline size_t getWdrRelayServerInfoSize(const RelayServerInfo& /*info*/) {
    return Wdr::sizeShort + getWdrServerStateSize();
}

// = ErrorCode

inline bool readWdrErrorCode(Wdr::InputWdr& wdr, ErrorCode& errorCode) {
    Wdr::Long code;
    if (! wdr.read(code))
        return false;
    errorCode = static_cast<ErrorCode>(code);
    return true;
}

inline bool writeWdrErrorCode(Wdr::OutputWdr& wdr, ErrorCode errorCode) {
    return wdr.write(static_cast<Wdr::Long>(errorCode));
}

inline size_t getWdrErrorCodeSize() {
    return Wdr::sizeLong;
}

// = UserNo

inline bool readWdrUserNo(Wdr::InputWdr& wdr, UserNo& userNo) {
    Wdr::Long user_no;
    if (! wdr.read(user_no))
        return false;
    userNo = toUserNo(user_no);
    return true;
}

inline bool writeWdrUserNo(Wdr::OutputWdr& wdr, UserNo userNo) {
    return wdr.write(static_cast<Wdr::Long>(userNo));
}

inline size_t getWdrUserNoSize() {
    return Wdr::sizeLong;
}

// = UserNos

bool WARNET_API readWdrUserNos(Wdr::InputWdr& wdr, UserNos& userNos);
bool WARNET_API writeWdrUserNos(Wdr::OutputWdr& wdr,
    const UserNos& userNos);
size_t WARNET_API getWdrUserNosSize(const UserNos& userNos);

// = Sex

inline bool readWdrSex(Wdr::InputWdr& wdr, Sex& sex) {
    Wdr::Octet s;
    if (! wdr.read(s))
        return false;
    sex = static_cast<Sex>(s);
    return isValidSex(sex);
}

inline bool writeWdrSex(Wdr::OutputWdr& wdr, Sex sex) {
    return wdr.write(static_cast<Wdr::Octet>(sex));
}

inline size_t getWdrSexSize() {
    return Wdr::sizeOctet;
}

// = UserState

inline bool readWdrUserState(Wdr::InputWdr& wdr, im::UserState& userState) {
    Wdr::Octet state;
    if (! wdr.read(state))
        return false;
    userState = static_cast<im::UserState>(state);
    return isValidUserState(userState);
}

inline bool writeWdrUserState(Wdr::OutputWdr& wdr, im::UserState userState) {
    return wdr.write(static_cast<Wdr::Octet>(userState));
}

inline size_t getWdrUserStateSize() {
    return Wdr::sizeOctet;
}

// = RoomId

inline bool readWdrRoomId(Wdr::InputWdr& wdr, RoomId& idRoom) {
    Wdr::Long id;
    if (! wdr.read(id))
        return false;
    idRoom = toRoomId(id);
    return true;
}

inline bool writeWdrRoomId(Wdr::OutputWdr& wdr, RoomId idRoom) {
    return wdr.write(static_cast<Wdr::Long>(idRoom));
}

inline size_t getWdrRoomIdSize() {
    return Wdr::sizeLong;
}

// = GameRoundId

inline bool readWdrGameRoundId(Wdr::InputWdr& wdr,
        GameRoundId& idGameRound) {
    Wdr::Short id;
    if (! wdr.read(id))
        return false;
    idGameRound = toGameRoundId(id);
    return true;
}

inline bool writeWdrGameRoundId(Wdr::OutputWdr& wdr,
        GameRoundId idGameRound) {
    return wdr.write(static_cast<Wdr::Short>(idGameRound));
}

inline size_t getWdrGameRoundIdSize() {
    return Wdr::sizeShort;
}


// = GameRoundIds

bool WARNET_API readWdrGameRoundIds(Wdr::InputWdr& wdr,
    GameRoundIds& gameRoundIds);
bool WARNET_API writeWdrGameRoundIds(Wdr::OutputWdr& wdr,
    const GameRoundIds& gameRoundIds);
size_t WARNET_API getWdrGameRoundIdsSize(
    const GameRoundIds& gameRoundIds);

// = GameModeType

inline bool readWdrGameMode(Wdr::InputWdr& wdr, GameModeType& gameMode) {
    Wdr::Octet mode;
    if (! wdr.read(mode))
        return false;
    gameMode = static_cast<GameModeType>(mode);
    return isValidGameMode(gameMode);
}

inline bool writeWdrGameMode(Wdr::OutputWdr& wdr, GameModeType gameMode) {
    return wdr.write(static_cast<Wdr::Octet>(gameMode));
}

inline size_t getWdrGameModeSize() {
    return Wdr::sizeOctet;
}

// = RoomType

inline bool readWdrRoomType(Wdr::InputWdr& wdr, RoomType& roomType) {
    Wdr::Octet mode;
    if (! wdr.read(mode))
        return false;
    roomType = static_cast<RoomType>(mode);
    return isValidRoomType(roomType);
}

inline bool writeWdrRoomType(Wdr::OutputWdr& wdr, RoomType roomType) {
    return wdr.write(static_cast<Wdr::Octet>(roomType));
}

inline size_t getWdrRoomTypeSize() {
    return Wdr::sizeOctet;
}

// = GameType

inline bool readWdrGameType(Wdr::InputWdr& wdr, GameType& gameType) {
    Wdr::Octet type;
    if (! wdr.read(type))
        return false;
    gameType = static_cast<GameType>(type);
    return isValidGameType(gameType);
}

inline bool writeWdrGameType(Wdr::OutputWdr& wdr, GameType gameType) {
    return wdr.write(static_cast<Wdr::Octet>(gameType));
}

inline size_t getWdrGameTypeSize() {
    return Wdr::sizeOctet;
}

// = GameGenre

inline bool readWdrGameGenre(Wdr::InputWdr& wdr, GameGenre& gameGenre) {
    Wdr::Octet genre;
    if (! wdr.read(genre))
        return false;
    gameGenre = static_cast<GameGenre>(genre);
    return isValidGameGenre(gameGenre);
}

inline bool writeWdrGameGenre(Wdr::OutputWdr& wdr, GameGenre gameGenre) {
    return wdr.write(static_cast<Wdr::Octet>(gameGenre));
}

inline size_t getWdrGameGenreSize() {
    return Wdr::sizeOctet;
}

// = GameRoomState

inline bool readWdrGameRoomState(Wdr::InputWdr& wdr,
    GameRoomState& gameRoomstate) {
    Wdr::Octet state;
    if (! wdr.read(state))
        return false;
    gameRoomstate = static_cast<GameRoomState>(state);
    return isValidGameRoomState(gameRoomstate);
}

inline bool writeWdrGameRoomState(Wdr::OutputWdr& wdr,
    GameRoomState gameRoomstate) {
    return wdr.write(static_cast<Wdr::Octet>(gameRoomstate));
}

inline size_t getWdrGameRoomStateSize() {
    return Wdr::sizeOctet;
}

// = Grade

inline bool readWdrGrade(Wdr::InputWdr& wdr, Grade& grade) {
    Wdr::Octet flag;
    if (! wdr.read(flag))
        return false;
    grade = static_cast<Grade>(flag);
    return isValidGrade(grade);
}

inline bool writeWdrGrade(Wdr::OutputWdr& wdr, Grade grade) {
    return wdr.write(static_cast<Wdr::Octet>(grade));
}

inline size_t getWdrGradeSize() {
    return Wdr::sizeOctet;
}

// = Coin

inline bool readWdrCoin(Wdr::InputWdr& wdr, Coin& coin)
{
    return wdr.read(coin);
}

inline bool readWdrPoint(Wdr::InputWdr& wdr, Cash& coin)
{
    return wdr.read(coin);
}

inline bool writeWdrCoin(Wdr::OutputWdr& wdr, Coin coin)
{
    return wdr.write(coin);
}

inline bool writeWdrPoint(Wdr::OutputWdr& wdr, Cash coin)
{
    return wdr.write(coin);
}

inline size_t getWdrCoinSize()
{
    return Wdr::sizeShort;
}

inline size_t getWdrPointSize()
{
    return Wdr::sizeLong;
}


// = GuildDuty

inline bool readWdrGuildDuty(Wdr::InputWdr& wdr, GuildDuty& duty)
{
    Wdr::Octet value;
    if (wdr.read(value)) {
        duty = static_cast<GuildDuty>(value);
    }
    return wdr.isGood();
}


inline bool writeWdrGuildDuty(Wdr::OutputWdr& wdr, GuildDuty duty)
{
    return wdr.write(static_cast<Wdr::Octet>(duty));
} 

inline size_t getWdrGuildDutySize()
{
    return Wdr::sizeOctet;
}

// = GuildInfo

inline bool readWdrGuildInfo(Wdr::InputWdr& wdr, GuildInfo& gi)
{
    return readWdrString(wdr, gi.guildName_, DL_MAX_GUILDNAME) &&
        readWdrGuildDuty(wdr, gi.guildDuty_);
}

inline bool writeWdrGuildInfo(Wdr::OutputWdr& wdr, const GuildInfo& gi)
{
    return writeWdrString(wdr, gi.guildName_) &&
        writeWdrGuildDuty(wdr, gi.guildDuty_);
}

inline size_t getWdrGuildInfoSize(const GuildInfo& gi)
{
    return getWdrStringSize(gi.guildName_) +
        getWdrGuildDutySize();
}

// = InvitationRejectionReason

inline bool readWdrInvitationRejectionReason(Wdr::InputWdr& wdr,
    InvitationRejectionReason& reason)
{
    Wdr::Octet value;
    if (wdr.read(value)) {
        reason = static_cast<InvitationRejectionReason>(value);
    }
    return wdr.isGood();
}

inline bool writeWdrInvitationRejectionReason(Wdr::OutputWdr& wdr,
    InvitationRejectionReason reason)
{
    return wdr.write(static_cast<Wdr::Octet>(reason));
}

inline size_t getWdrInvitationRejectionReasonSize()
{
    return Wdr::sizeOctet;
}

// = Rupo

inline bool readWdrRupo(Wdr::InputWdr& wdr, Rupo& rupo)
{
    return wdr.read(rupo);
}

inline bool writeWdrRupo(Wdr::OutputWdr& wdr, Rupo rupo)
{
    return wdr.write(rupo);
}

inline size_t getWdrRupoSize()
{
    return Wdr::sizeLong;
}

// = ItemId

inline bool readWdrItemId(Wdr::InputWdr& wdr, ItemId& itemId)
{
    Wdr::Long value;
    if (! wdr.read(value))
        return false;
    itemId = static_cast<ItemId>(value);
    return isValidItemId(itemId);
}


inline bool writeWdrItemId(Wdr::OutputWdr& wdr, ItemId itemId)
{
    return wdr.write(static_cast<Wdr::Long>(itemId));
}


inline size_t getWdrItemIdSize()
{
    return Wdr::sizeLong;
}

// = ItemCode

inline bool readWdrItemCode(Wdr::InputWdr& wdr, ItemCode& itemCode)
{
    Wdr::Long value;
    if (! wdr.read(value))
        return false;
    itemCode = static_cast<ItemCode>(value);
    //return isValidItemCode(itemCode); ÇÏ¸é ¾ÈµÊ!!
    return true;
}


inline bool writeWdrItemCode(Wdr::OutputWdr& wdr, ItemCode itemCode)
{
    return wdr.write(static_cast<Wdr::Long>(itemCode));
}


inline size_t getWdrItemCodeSize()
{
    return Wdr::sizeLong;
}

// = PoporuServersInfo

inline bool readWdrPoporuServersInfo(Wdr::InputWdr& wdr,
    PoporuServersInfo& info) {
    return readWdrLoginServerInfo(wdr,info.loginServerInfo_) &&
        readWdrGameServerInfoMap(wdr, info.gameServerInfoMap_);
}

    
inline bool writeWdrPoporuServersInfo(Wdr::OutputWdr& wdr,
    const PoporuServersInfo& info) {
    return writeWdrLoginServerInfo(wdr,info.loginServerInfo_) &&
        writeWdrGameServerInfoMap(wdr, info.gameServerInfoMap_);
}

    
inline size_t getWdrPoporuServersInfoSize(const PoporuServersInfo& info) {
    return getWdrLoginServerInfoSize(info.loginServerInfo_) + 
        getWdrGameServerInfoMapSize(info.gameServerInfoMap_);
}

// = channelId

inline bool readWdrChannelId(Wdr::InputWdr& wdr, ChannelId& channelId) {
    Wdr::Long value;
    if (! wdr.read(value))
        return false;
    channelId = static_cast<ChannelId>(value);
    return wdr.isGood();
}


inline bool  writeWdrChannelId(Wdr::OutputWdr& wdr,ChannelId channelId) {
    return wdr.write(static_cast<Wdr::Long>(channelId));
}


inline size_t getWdrChannelIdSize() {
    return Wdr::sizeLong;
}

// = ServerState

inline bool readWdrServerState(Wdr::InputWdr& wdr, 
    ServerState& serverState) {
    Wdr::Octet value;
    if (! wdr.read(value))
        return false;
    serverState = static_cast<ServerState>(value);
    return wdr.isGood();
}


inline bool writeWdrServerState(Wdr::OutputWdr& wdr,
    const ServerState& serverState){
        return wdr.write(static_cast<Wdr::Octet>(serverState));
}


inline size_t getWdrServerStateSize() {
    return Wdr::sizeOctet;
}


// = ForwardServerInfo
inline bool readWdrForwardServerInfo(Wdr::InputWdr& wdr, 
    ForwardServerInfo& forwardServerInfo) {
    if( !readWdrServerState(wdr,forwardServerInfo.serverState_))
        return false;
    return wdr.isGood();
}


inline bool writeWdrForwardServerInfo(Wdr::OutputWdr& wdr,
    const ForwardServerInfo& forwardServerInfo){
        return writeWdrServerState(wdr,forwardServerInfo.serverState_);
}


inline size_t getWdrForwardServerInfoSize() {
    return Wdr::sizeOctet;
}



// = CandyInfos

//bool WARNET_API readWdrCandyInfos(Wdr::InputWdr& wdr, CandyInfos& candyInfos);
//bool WARNET_API writeWdrCandyInfos(Wdr::OutputWdr& wdr,
//                                   const CandyInfos& candyInfos);
//size_t WARNET_API getWdrCandyInfosSize(const CandyInfos& candyInfos);

} // namespace skynet

#endif // !defined(INCLUDED_POPORUWDRUTIL)
