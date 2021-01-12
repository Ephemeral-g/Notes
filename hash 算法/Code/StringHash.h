#pragma once

#include <string>

constexpr size_t maxTableLen = 1024;

typedef struct _HASHTABLE
{
	long hashA;
	long hashB;
	bool exist;
}HASHTABLE, *PHASHTABLE;

class StringHash
{
public:
	StringHash(const long tableLen = maxTableLen);
	~StringHash();

public:
	bool Hash(const std::string &str);
	unsigned long Hashed(const std::string &str);  // 检测str是否被hash过

private:
	void InitCryptTable(); // 对哈希索引表预处理
	unsigned long HashString(const std::string &str, unsigned long nHashType);  // 求取哈希值

private:
	unsigned long cryptTable[0x500];
	unsigned long m_tableLen;  // 哈希索引表长度
	HASHTABLE *m_hashTable;
};