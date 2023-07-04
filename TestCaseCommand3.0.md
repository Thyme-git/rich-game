# 游戏机制：
```
回合初的触发顺序：地点效果（购买空地，缴税，进入道具屋，魔法屋，监狱，礼品屋）,buff结算（财神和轮空状态），玩家行动
回合初触发的地点效果：得到财神和轮空状态时须在基础数值上+1
buff结算：回合开始时buff结算判定当前财神和轮空状态，若不为0则减一

回合中：
玩家可以输入指令的时候就是回合中
如sell，roll等

回合后（roll点后）：
炸弹等道具在roll之后，前往目标地点的过程中（包括目标地点）碰到立即触发
到下一个玩家的回合初

测试要求：
如果你希望测试某个地点效果（不包括道具效果），在其他玩家回合修改目标玩家位置以在目标玩家回合开始时能够触发特别效果
炸弹不能放在玩家存在的地方，不能传送到炸弹等道具存在的地方（路障同理）
```


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

### set item [A|Q|S|J] [(item)1|2|3] [num] // 设置玩家物品

```
set item A 1 10 // 设置阿土伯的路障数量为10

set item Q 2 2 // 设置钱夫人的机器娃娃数量为2

set item J 3 5  // 设置金贝贝的炸弹数量为5
```

### set buff [A|Q|S|J] [n] // 设置是否有财神

```
set buff A n // 给予阿土伯财神buff

set buff Q 0 // 除去钱夫人的财神buff(如果没buff则无效)
```

## 地图相关

### set map [n] [A|Q|S|J] [level] // 设置可购买的地皮n属于某玩家并有level等级(0|1|2|3)的房产

```
set map 5 A 2 // 设置5号土地属于阿土伯，并建立了洋房

set unmap [n] // 将可购买的地皮n设置为未购买状态 

set bomb [n] // 在可放置炸弹的地皮n上放置炸弹

set barrier [n] // 在可放置障碍的地皮n上放置障碍

set pos [A|Q|S|J] [n] // 将玩家移动到地皮n


```
## 玩家相关
```
step [n] // 让当前玩家向前后某方向移动n格，正数为前进，负数后退,效果就是一个能指定步数的roll,step后就不能roll

set quit [A|Q|S|J] // 让非当前玩家强制退出，系统回收地皮

set stop [A|Q|S|J] [n] //设置玩家停留时间 
```
## 输出相关
## dump文件格式
```
user [A|Q|S|J] //游戏中的玩家，要求顺序
A//玩家属性
alive [0|1]//0为出局，1为游戏中
money [n]
point [n]
item1 [n]//路障
item2 [n]//娃娃
item3 [n]//炸弹
buff [n]//财神剩余天数
stop [n]//n为剩余停留天数
userloc [n]
Q
alive [0|1]
money [n]
point [n]
item1 [n]//路障
item2 [n]//娃娃
item3 [n]//炸弹
buff [n]//财神剩余天数
stop [n]//n为剩余停留天数
userloc [n]
S
alive [0|1]
money [n]
point [n]
item1 [n]//路障
item2 [n]//娃娃
item3 [n]//炸弹
buff [n]//财神剩余天数
stop [n]//n为剩余停留天数
userloc [n]
J
alive [0|1]
money [n]
point [n]
item1 [n]//路障
item2 [n]//娃娃
item3 [n]//炸弹
buff [n]//财神剩余天数
stop [n]//n为剩余停留天数
userloc [n]
MAP//地图属性
//地图属性的条数是可变的，仅记录和初始地图不一致的地块
mapuser [n] [AQSJ]//地块n上的玩家（由先到达的排在前）
...
map [n] [A|Q|S|J] [level]//地块n上某玩家拥有等级为level的房屋
...
item [n] [1|3]//地块n上存在某个道具
...

```