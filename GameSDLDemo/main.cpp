
#include "Common_Function.h"
#include "MainObject.h"
#include"ThreatsObject.h"
#include"ExplosionObject.h"

bool Init()
{
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
  {
    return false;
  }

  g_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

  if (g_screen == NULL)
    return false;

  return true;
}


int main(int arc, char*argv[])
{
	bool is_run_screen = true;
	int bkgn_x = 0;
  bool is_quit = false;
  if (Init() == false)
    return 0;

  g_bkground = SDLCommonFunc::LoadImage("bg4800.png");
  if (g_bkground == NULL)
  {
    return 0;
  }
 
  MainObject plane_object;
  plane_object.SetRect(100, 200);
  bool ret = plane_object.LoadImg("plane_fly.png");
  if (!ret)
  {
    return 0;
  }

  ExplosionObject exp_main;
  ret = exp_main.LoadImg("exp_main.png");
  exp_main.set_clip();
  if (ret == false) return 0;




  ThreatsObject* p_threats = new ThreatsObject[NUM_THREATS];
  for (int t=0; t < NUM_THREATS ; t++)
  {
	  ThreatsObject* p_threat = (p_threats + t);
		  if(p_threat)
		  {
		  ret= p_threat->LoadImg("af1.png");
  if(ret == false)
  {
  return 0;

  }
  int rand_y =rand()%400;
		if ( rand_y > SCREEN_HEIGHT - 200)
		{
			rand_y= SCREEN_HEIGHT*0.3;
		}

		

		p_threat->SetRect(SCREEN_WIDTH + t*400,rand_y);
        p_threat->set_x_val(3); 

		AmoObject* p_amo = new AmoObject();
		p_threat->InitAmo(p_amo);
  }
  }

  
	
	
	
		



  while (!is_quit) 
  {
    while (SDL_PollEvent(&g_even)) 
    {
      if (g_even.type == SDL_QUIT)
      {
        is_quit = true;
        break;
      }
      plane_object.HandleInputAction(g_even);
    }
	// bkgn_x -= 2;
	//SDLCommonFunc::ApplySurface(g_bkground, g_screen, bkgn_x, 0);
	//SDLCommonFunc::ApplySurface(g_bkground,g_screen,bkgn_x + SCREEN_WIDTH,0);
	//if (bkgn_x <= -SCREEN_WIDTH )
	//{
		//bkgn_x= 0;
	//}
	if (is_run_screen == true )
	{
	bkgn_x -= 2;
	
	if (bkgn_x <= -(WIDTH_BACKGROUND - SCREEN_WIDTH))
	{
		is_run_screen = false ;
	}
	else
	{
	SDLCommonFunc::ApplySurface(g_bkground, g_screen, bkgn_x,0);
	}
	}
	else 
	{
	SDLCommonFunc::ApplySurface(g_bkground, g_screen, bkgn_x,0);
	}


	plane_object.HandleMove();
    plane_object.Show(g_screen);
	

    for (int i = 0; i < plane_object.GetAmoList().size(); i++)
    {
      std::vector<AmoObject*> amo_list = plane_object.GetAmoList();
      AmoObject* p_amo = amo_list.at(i);
      if (p_amo != NULL)
      {
        if (p_amo->get_is_move())
        {
          p_amo->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
          p_amo->Show(g_screen);
        }
        else 
        {
          if (p_amo != NULL)
          {
            amo_list.erase(amo_list.begin() + i);
            plane_object.SetAmoList(amo_list);

            delete p_amo;
            p_amo = NULL;
          }
        }
      }
    }
	//run threats
	for (int tt=0; tt < NUM_THREATS; tt ++)
	{
		ThreatsObject* p_threat = (p_threats +tt);
		if ( p_threat)
		{
    p_threat->HandleMove(SCREEN_WIDTH,SCREEN_HEIGHT);
	p_threat->Show(g_screen);
	p_threat->MakeAmo(g_screen,SCREEN_WIDTH,SCREEN_HEIGHT);

	if ( SDL_Flip(g_screen) == -1)
      return 0;

	bool is_col = SDLCommonFunc::CheckCollision(plane_object.GetRect(),p_threat->GetRect());
	if (is_col)
	{
		for(int ex =0; ex < 4; ex ++)
		{
			int x_pos = (plane_object.GetRect().x + plane_object.GetRect().w * 0.5) -EXP_WIDTH*0.5;
			int y_pos =(plane_object.GetRect().y + plane_object.GetRect().h * 0.5) -EXP_HEIGHT*0.5;

			exp_main.set_frame(ex);
			exp_main.SetRect(x_pos,y_pos);
			exp_main.ShowEx(g_screen);
			SDL_Delay(100);
			
    if ( SDL_Flip(g_screen) == -1)
      return 0;
		
		}
		if (MessageBox(NULL, L"Game Over!", L"Info", MB_OK)== IDOK)
		{
		delete [] p_threats;

  SDLCommonFunc::CleanUp();
  SDL_Quit();
  return 1;
		}
	}

	std::vector<AmoObject*> amo_list = plane_object.GetAmoList();
	for (int im =0; im <amo_list.size(); im ++)
	{
		AmoObject* p_amo = amo_list.at(im);
		if (p_amo != NULL)
		{
			bool ret_col = SDLCommonFunc::CheckCollision(p_amo->GetRect(), p_threat->GetRect());
			if (ret_col)
			{
				p_threat->Reset(SCREEN_WIDTH + tt*400);
				plane_object.RemoveAmo(im);
			}
		}
	}


	    }
	}

    if ( SDL_Flip(g_screen) == -1)
      return 0;
  }


  delete [] p_threats;

  SDLCommonFunc::CleanUp();
  SDL_Quit();

  return 1;
}