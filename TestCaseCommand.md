[TOC]

# 大富翁游戏测试用指令

## 游戏初始化相关

### set player [AQSJ]  // 设置初始游戏玩家


```
set player AQJ // 设置初始游戏玩家数为3，玩家为阿土伯、钱夫人、金贝贝

set player SJ // 设置初始游戏玩家数为2，玩家为孙小美、金贝贝
```

## 物品、道具相关

### set money [A|Q|S|J] [+|-| ] [value] // 设置玩家资金

```
set money A 10000 // 设置阿土伯的资金为10000

set money S 50000 // 设置孙小美资金为50000

set money Q +500  // 设置钱夫人资金加500
```

### set point [A|Q|S|J] [+|-| ] [value] // 设置玩家点数

```
set point J 222 // 设置金贝贝的点数为222

set point S +80 // 设置孙小美点数加80

set point A -500  // 设置阿土伯点数减500
```

### set Item [A|Q|S|J] [(item)1|2|3] [num] // 设置玩家物品

```
set Item A 1 10 // 设置阿土伯的路障数量为10

set Item Q 2 2 // 设置钱夫人的机器娃娃数量为2

set Item J 3 5  // 设置金贝贝的炸弹数量为5
```

### set Buff [A|Q|S|J] [1|0] // 设置是否有财神

```
set Buff A 1 // 给予阿土伯财神buff

set Buff Q 0 // 除去钱夫人的财神buff(如果没buff则无效)
```

## 地图相关

### set map [n] [A|Q|S|J] [level] // 设置可购买的地皮n属于某玩家并有level等级(0|1|2|3)的房产

```
set map 5 A 2 // 设置5号土地属于阿土伯，并建立了洋房
```

### set unmap [n] // 将可购买的地皮n设置为未购买状态 

### set bomb [n] // 在可放置炸弹的地皮n上放置炸弹

### set barrier [n] // 在可放置障碍的地皮n上放置障碍

### set pos [A|Q|S|J] [n] // 将玩家移动到地皮n

### set hospital [A|S|Q|J] // 将玩家击杀，送至医院

### set prison [A|S|Q|J] // 让玩家走到监狱，扣留玩家

## 玩家相关

### step [+|-] [n] // 让当前玩家向前后某方向移动n格，正数为前进，负数后退

### set quit [A|Q|S|J] // 让玩家强制退出，系统回收地皮

## 输出相关

### set dump // 输出内存信息（还没想好格式）