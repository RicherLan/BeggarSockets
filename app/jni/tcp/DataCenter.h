/** 
 * author: BeggarLan
 * created on: 2023/7/6 15:23 
 * description: 管理数据
 */

#ifndef BEGGARSOCKETS_DATACENTER_H
#define BEGGARSOCKETS_DATACENTER_H

#include "map"

// todo 大杂烩，应该分角色
class DataCenter {
public:
    DataCenter();
    ~DataCenter();

    // todo 地址封装
    // 添加一组地址
    void addAddressAndPort(std::string address, uint16_t port);
    // 获得当前的地址
    std::string getCurrentAddress();
    uint16_t  getCurrentPort();

private:
    // 所有的地址
    // todo 合适的数据结构
    std::vector<std::string> addresses;
    std::map<std::string, uint16_t> ports;
    // 当前地址索引
    uint32_t currentAddressIndex = 0;

};

#endif //BEGGARSOCKETS_DATACENTER_H
