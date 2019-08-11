# Handmade Hero Day029, Basic Tile Map Collision Checking

## 概述

继续上一天的工作成果进行主要处理的问题。
到day28为止，目前handmade hero可以绘制代表玩家的黄色方块，

### 绘制区域以外的颜色拉伸

1. resize的时候color buffer向没有绘制地方的拉伸的问题，以及正常渲染区域的闪烁问题
```
PatBlt(DeviceContext, 0, 0, WindowWidth, OffsetY, BLACKNESS);
PatBlt(DeviceContext, 0, OffsetY + Buffer->Height, WindowWidth, WindowHeight, BLACKNESS);
PatBlt(DeviceContext, 0, 0, OffsetX, WindowHeight, BLACKNESS);
PatBlt(DeviceContext, OffsetX + Buffer->Width, 0, WindowWidth, WindowHeight, BLACKNESS);
```
2. 让玩家控制的小方块可以实现与地图的碰撞

## 碰撞的检查

1. 基于单点的检查坐标的uv等于 (0.5, 1)
2. 基于多点的检查坐标的uv等于 (0, 1), (0.5, 1), (1, 1)
   * 小的改变（e.g., baby step 简单实现容易理解快速生效），看到游戏的碰撞的确变好了说明我们在朝一个正确的方向前进
   
```
if(IsTileMapPointEmpty(TileMap, NewPlayerX - 0.5f * PlayerWidth, NewPlayerY) &&
   IsTileMapPointEmpty(TileMap, NewPlayerX + 0.5f * PlayerWidth, NewPlayerY) &&
   IsTileMapPointEmpty(TileMap, NewPlayerX, NewPlayerY))
{
	GameState->PlayerX = NewPlayerX;
	GameState->PlayerY = NewPlayerY;
}
```
也许遍历一遍接下来要进入的tiles，依次进行判断其合法性

## 数据结构的设计先行?

Casey写代码开始都不会进行数据结构的设计，他倾向于在实现的时候完成相关的设计，
这样会对代码进行较多的重构。
对于整个TileMap的抽象TileMap

```
struct tile_map
{
    int32 CountX;
    int32 CountY;
    
    real32 UpperLeftX;
    real32 UpperLeftY;
    real32 TileWidth;
    real32 TileHeight;

    uint32 *Tiles;
};
```

## DENSE STORAGE V.S SPARSE STORAGE

结尾的时候无法较好的处理world地图的数据
```
struct world
{
    // TODO(casey): Beginner's sparseness
    int32 TileMapCountX;
    int32 TileMapCountY;
    
    tile_map *TileMaps;
};

```
	
# bugs 

* 玩家的移动的点实际上是离散的，目前Casey打算在以后介绍一套完整的系统，这个版本里面依旧留下了这个问题
  * 产生移动跨墙的问题
  * 与墙之间产生过大的空隙
* dll loaded, but not the pdb
* tile data encoding 
* 对角的移动可能会更加的快速根号二倍的速度
* platform_header
