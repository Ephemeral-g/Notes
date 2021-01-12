# 字符串 hash 算法

暴雪公司有一个经典的字符串的 hash 公式。

所谓 hash，一般是一个整数，通过某种算法，可以把一个字符串“压缩”成一个整数，这个数称为 hash，当然，无论如何，一个32位整数是无法对应回一个字符串的，但在程序中，两个字符串计算出的 hash 值相等的可能性非常小，在 MPQ 中的 hash 算法：

```c++
unsigned long HashString(char *pStr, unsigned long nHashType) {
    unsigned char *key = (unsigned char*)pStr;
    unsigned long seed1 = 0x7FED7FED, seed2 = 0xEEEEEEEE;
    int ch;
    while (*key != 0) {
        ch = toupper(*key++);
        seed1 = cryptTable[(nHashType << 8) + ch] ^ (seed1 + seed2);
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
    }
    return seed1;
}
```

Blizzard 的这个算法是非常高效的，被称为“One-Way Hash”，举个例子，字符串“unitneutralacritter.grp”通过这个算法得到的结果是 0xA26067F3。

通常构造一个哈希表（Hash Table），哈希表是一个大数组，这个数组的容量根据程序的要求来定义，例如1024，每一个 Hash 值通过取模运算（mod）对应到数组中的一个位置，这样，只要比较这个字符串的哈希值对应的位置又没有被占用，就可以得到最后的结果了，想想这是什么速度？最快的O(1)。

```c++
int GetHashTablePos(char *pString, SOMESTRUCTURE *pTable, int nTableSize) {
    int nHash = HashString(pString), nHashPos = nHash % nTableSize;
    if (pTable[nHashPos].bExists && !strcmp(pTable[nHashPos].pString, pString))
		return nHashPos;
    else
        return -1;
}
```

但是存在一个很严重的问题：“假如两个字符串在哈希表中对应的位置相同怎么办？”，毕竟一个数组容量是有限的，这种可能性很大。解决该问题的方法很多，首先就是用“链表”，很多算法都可以转化成链表来解决，只要在哈希表的每个入口挂一个链表，保存所有对应的字符串就 OK 了。

然而 Blizzard 的程序员使用的方法则是更精妙的方法。基本原理就是：他们在哈希表中不是用一个哈希值而是用三个哈希值来校验字符串。

中国有句古话“再一再二不能再三再四”。假如说两个不同的字符串经过一个哈希算法得到的入口点一致有可能，但用三个不同的哈希算法算出的入口点都一致，那几乎可以肯定是不可能的事了，这个几率是1:18889465931478580854784，大概是10的22.3次方分之一。

Blizzard 使用的哈希表没有使用链表，而采用“顺延”的方式来解决问题，看看这个算法：

```c++
int GetHashTablePos(char *pString, MPQHASHTABLE *pTable, int nTableSize) {
    const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
    int nHash = HashString(pString, HASH_OFFSET);
    int nHashA = HashString(pString, HASH_A);
    int nHashB = HashString(pString, HASH_B);
    int nHashStart = nHash % nTableSize, nHashPos = nHashStart;
    while (pTable[nHashPos].bExits)
    {
        if (pTable[nHashPos].nHashA == nHashA && pTable[nHashPos].nHashB == nHashB)
            return nHashPos;
        else
            nHashPos = (nHashPos + 1) % nTableSize;
        if (nHashPos == nHashStart)
            break;
    }
    return -1;
```

1. 计算出字符串的三个哈希值（一个用来确定位置，另外两个用来校验）
2. 查看哈希表中的这个位置，假如为空，则该字符串不存在，返回
3. 假如存在，则检查其他两个哈希值是否也匹配，假如匹配，则表示找到了该字符串，返回
4. 否则移到下一个位置，判断是不是又回到了原来的位置，假如是，则没找到
5. 否则回到3

完整算法详见 StringHash 文件

