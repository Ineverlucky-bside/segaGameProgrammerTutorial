#include <iostream>

//#墙 _空白区 .终点 o砖块 p人
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";

const int gStageWidth = 8;
const int gStageHeight = 5;

enum Object
{
  OBJ_SPACE,
  OBJ_WALL,
  OBJ_GOAL,
  OBJ_BLOCK,
  OBJ_BLOCK_ON_GOAL,
  OBJ_MAN,
  OBJ_MAN_ON_GOAL,

  OBJ_UNKNOWN,
};

void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);

int main()
{
  //创建初始化数组
  Object* state = new Object[gStageWidth * gStageHeight];
  //初始化场景
  initialize(state, gStageWidth, gStageHeight, gStageData);
  //主循环

  while (true)
  {
    //首先绘制
    draw(state, gStageWidth, gStageHeight);
    //通关检测
    if (checkClear(state, gStageWidth, gStageHeight))
    {
      break;
    }

    //获取输入
    std::cout << "a:left s:right w:up z:down. command?" << std::endl;
    char input;
    std::cin >> input;

    //更新
    update(state, input, gStageWidth, gStageHeight);
  }

  //胜利时提示信息
  std::cout << "Congratuation's! You Win. " << std::endl;
  //通关
  delete[] state; // 通过new创建的数组不能使用delete，而应当使用delete[]
  state = 0; //置空指针

  return 0;
}


void initialize(Object* state, int width, int height, const char* stageData)
{
  const char* d = stageData;//读取指针
  int x = 0;
  int y = 0;
  while (*d != '\0')
  {
    //当字符不为空时
    Object t;
    switch (*d)
    {
    case '#': t = OBJ_WALL; break;
    case ' ': t = OBJ_SPACE; break;
    case 'o': t = OBJ_BLOCK; break;
    case 'O': t = OBJ_BLOCK_ON_GOAL; break;
    case '.': t = OBJ_GOAL; break;
    case 'p': t = OBJ_MAN; break;
    case 'P': t = OBJ_MAN_ON_GOAL; break;
    case '\n': //到下一行
      x = 0; //x返回到最左
      ++y; //y前进到下一段
      t = OBJ_UNKNOWN; //没有数据
      break;
    default: t = OBJ_UNKNOWN; break;//非法数据
    }
    ++d;
    //如果遇到未知字符则无视
    if (t != OBJ_UNKNOWN)
    {
      state[y * width + x] = t; //写入
      ++x;
    }
  }
}

void draw(const Object* state, int width, int height)
{
  const char font[] = { ' ', '#', '.', 'o', 'O', 'p', 'P' };
  for (int y = 0; y < height; ++y)
  {
    for (int x = 0; x < width; ++x)
    {
      Object o = state[y * width + x];
      std::cout << font[o];
    }
    std::cout << std::endl;
  }
}

void update(Object* s, char input, int w, int h)
{
  int dx = 0;
  int dy = 0;
  switch (input)
  {
  case 'a' : dx = -1; break;
  case 's': dx = 1; break;
  case 'w': dy = -1; break;
  case 'z': dy = 1; break;
  }

  //查询角色的坐标
  int i = -1;
  for (i = 0; i < w * h; ++i)
  {
    if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL)
    {
      break;
    }
  }
  int x = i % w;
  int y = i / w;

  //移动
  //移动后的坐标
  int tx = x + dx;
  int ty = y + dy;
  //判断坐标的极端值，不允许超出合理范围
  if (tx < 0 || tx >= w || ty < 0 || ty >= h)
  {
    return;
  }

  //A.该方向上是空白或者终点，角色移动
  int p = y * w + x;//角色实际位置
  int tp = (ty * w) + tx; //角色目标位置
  if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL)
  {
    s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN; //如果目标位置是终点，位置值改为OBJ_MAN_ON_GOAL
    s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE; //修改当前的位置
  }
  //B. 如果这个方向上是箱子，并且这个方向上下下个格子是空白或者终点，则允许移动
  else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL)
  {
    //首先检测同方向上的下下个格子是否位于合理的范围
    int tx2 = tx + dx;
    int ty2 = ty + dy;
    if (tx2 < 0 || tx2 >= w || ty2 < 0 || ty2 >= h)
    {
      return;
    }

    int tp2 = (ty + dy) * w + (tx + dx); //下下个格子
    if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL)
    {
      //按顺序替换
      s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
      s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
      s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
    }
  }
}

//如果没有方块了则判定为通关
bool checkClear(const Object* s, int width, int height)
{
  for (int i = 0; i < width * height; ++i)
  {
    if (s[i] == OBJ_BLOCK)
    {
      return false;
    }
  }
  return true;
}





















