#include "stdafx.h"
#include "game.h"
ID3DXSprite* sprite;
std::shared_ptr<IDirect3DTexture9> texture;
static IDirect3DDevice9* Device;
bool Game::CancelTrue;
bool exitinggame;
//This class handles all game content and chooses when to activate certain events (Menu stages etc).
void Game::Initialize(IDirect3DDevice9Ptr m_Device, World* _World)
{
		this->world = _World;
		this->MouseX,this->MouseY = 0;
		Device = m_Device;
		D3DXCreateSprite(Device, &sprite);
		this->FPS = 60;
		int FontSize = 14;
		D3DXCreateFont(Device, 16, 0, FW_DONTCARE, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &this->DefaultFont);
		D3DXCreateFont(Device, -11, 0, FW_THIN, 0, FALSE, DEFAULT_CHARSET, OUT_SCREEN_OUTLINE_PRECIS, NONANTIALIASED_QUALITY, FIXED_PITCH | FF_ROMAN, TEXT("MS Sans Serif"), &this->MessageFont);

		this->ResourceManager = new Resource_Manager();
		this->ResourceManager->Initialize(Device);
		this->ScrollBarTexture  = this->ResourceManager->CreateTexture(2, 29, false);
		this->TextIconTexture = this->ResourceManager->CreateTexture(2, 32, true);
		menu = new Menu();
		map = new Map();
		GLoginButtonTexture = this->ResourceManager->CreateTexture(1, 15, true).Texture;
		this->ExitGameTxt = this->ResourceManager->CreateTexture(2, 39, true);
		this->MessageBoxTexture = this->ResourceManager->CreateTexture(1, 18, false);
		this->BT_Message_OK = new Button(this, MessageX + 180, MessageY + 112, 0, 116, 91, 28, false, GLoginButtonTexture);
		this->BT_ExitGame = new Button(this, 640 - 51, 0, 0, 0, 50, 53, true, this->ExitGameTxt.Texture);
		this->BT_CharDeleteOK = new Button(this, MessageX + 180 - 92, MessageY + 112, 0, 116, 91, 28, false, GLoginButtonTexture);
		this->BT_CharDeleteCancel = new Button(this, MessageX + 180, MessageY + 112, 0, 29, 91, 28, false, GLoginButtonTexture);
		Map_UserInterface = new Map_UI();
		menu->Initialize(world, Device, (LPVOID*)this);
		map->Initialize(world, Device, (LPVOID*)this);
		Map_UserInterface->Initialize(world, Device, (LPVOID*)this);


		
		this->MessageDragging = false;
		this->MessageSelected = false;
		this->MessageX = 190;
		this->MessageY = 150;
		this->Closed = false;
		this->Stage = Game::PMenu;
		this->SubStage = 0;
		this->CancelTrue = false;
		this->MsgID = 0;

		this->MapCursor = Map_UI_Cursor(this, this->map, Device);

		this->ENF_File = new ENF("pub\\dtn001.enf");
		this->EIF_File = new EIF("pub\\dat001.eif");
		this->ESF_File = new ESF("pub\\dsl001.esf");
		this->ECF_File = new ECF("pub\\dat001.ecf");

		this->world->W_UI_Infobox = new UI_InformationBox(this);	

}	

void World::ThrowMessage(std::string Title, std::string Message)
{
	World::MBTitle = Title;
	World::MBMessage = Message;
	World::MBHidden = false;
}

void World::DebugPrint(pt::string Message)
{
#ifdef DEBUG
	std::printf(Message + '\n');
#endif
	
}
void World::ThrowMessage(std::string Title, std::string Message, bool Okay)
{
	World::MBTitle = Title;
	World::MBMessage = Message;
	World::MBHidden = false;
	Game::CancelTrue = Okay;
}
float timerrender;
std::chrono::time_point<std::chrono::high_resolution_clock> init, final;
void Game::Update()
{
	this->world->W_UI_Infobox->Update();
	init = std::chrono::high_resolution_clock::now();
	if(!world->Connected && Stage != Game::PViewCredits)
	{
		Stage = Game::GameStage::PMenu;
	}
	if(World::MBHidden)
	{
		if (this->Stage < 5)
		{
			menu->Update();
		}
		else
		{
			this->MapCursor.Update();
			//if (World::UIBox_Hidden)
			{
				map->Update();
			}

			Map_UserInterface->Update();
		}
		
		if(this->Stage > 1)
		{ this->BT_ExitGame->Update(this->MouseX,this->MouseY,this->MousePressed); }
		
		if(this->BT_ExitGame->MouseClickedOnElement())
		{ 
			if(Stage == this->PViewCredits)
			{
				world->DropConnection();
				this->Stage = Game::PMenu;
				this->SubStage = 0;
				exitinggame = false;
				this->BT_ExitGame->MouseClickProccessed();
			}
			else
			{
			World::ThrowMessage("Return to menu","Are you sure you want to return to the \nmain menu?",true);
			exitinggame = true;
			this->BT_ExitGame->MouseClickProccessed();
			}

		}
	}
	else
	{
		menu->BT_CC_Delete1->SetFrameID(0);
		menu->BT_CC_Delete2->SetFrameID(0);
		menu->BT_CC_Delete3->SetFrameID(0);
		menu->BT_CC_Login1->SetFrameID(0);
		menu->BT_CC_Login2->SetFrameID(0);
		menu->BT_CC_Login3->SetFrameID(0);
		menu->BT_ChangePassCancel->SetFrameID(0);
		menu->BT_ChangePassOK->SetFrameID(0);
		menu->BT_CreateAccount->SetFrameID(0);
		menu->BT_CreateCharCancel->SetFrameID(0);
		menu->BT_CreateCharOK->SetFrameID(0);
		menu->BT_ExitGame->SetFrameID(0);
		menu->BT_LGCancel->SetFrameID(0);
		menu->BT_LGChangePass->SetFrameID(0);
		menu->BT_LGCreateChar->SetFrameID(0);
		menu->BT_LGPlayGame->SetFrameID(0);
		menu->BT_PlayGame->SetFrameID(0);
		menu->BT_ViewCredits->SetFrameID(0);
		if(!CancelTrue)
		{
			if (this->BT_Message_OK->GetFrameID() == 0 && this->MouseX > this->MessageX && this->MouseX < this->MessageX + 290 && this->MouseY > this->MessageY  && this->MouseY < this->MessageY  + 157)
			   {
				 if (this->RAWMousePressed)
					{
					   this->MessageSelected = true;
					}
			   }
		}
		else
		{
			if (this->BT_CharDeleteOK->GetFrameID() == 0  && this->BT_CharDeleteCancel->GetFrameID() == 0 && this->MouseX > this->MessageX && this->MouseX < this->MessageX + 290 && this->MouseY > this->MessageY  && this->MouseY < this->MessageY  + 157)
			   {
				 if (this->RAWMousePressed)
					{
					   this->MessageSelected = true;
					}
			   }
		}
        if (this->MessageSelected == true)
           {
             if (this->RAWMousePressed)
                {
                  if (this->MessageDragging == false)
                     {
                      DragX = this->MouseX - this->MessageX;
                      DragY = this->MouseY - this->MessageY ;
                      this->MessageDragging = true;
                     }
                  this->MessageX = this->MouseX - DragX;
                  this->MessageY  = this->MouseY - DragY;
                }
                else
                {
                 this->MessageDragging = false;
                 this->MessageSelected = false;
                }
           }
			this->BT_Message_OK->SetPosition(std::pair<int,int>(MessageX + 180,MessageY+ 112));
			this->BT_CharDeleteOK->SetPosition(std::pair<int, int>(MessageX + 180 - 92,MessageY+ 112));
			this->BT_CharDeleteCancel->SetPosition(std::pair<int, int>(MessageX + 180,MessageY+ 112));
			if(!this->MessageDragging)
			{
				if(!CancelTrue)
				{
					this->BT_Message_OK->Update(MouseX,MouseY,MousePressed);
					if(this->BT_Message_OK->MouseClickedOnElement())
					{
						World::MBHidden = true;
						this->BT_Message_OK->MouseClickProccessed();
					}
				}
				else
				{
					if(!exitinggame)
					{
						
					}
					this->BT_CharDeleteOK->Update(MouseX,MouseY,MousePressed);
					this->BT_CharDeleteCancel->Update(MouseX,MouseY,MousePressed);
					if(this->BT_CharDeleteOK->MouseClickedOnElement())
					{
						if(exitinggame)
						{	
							this->Stage = Game::PMenu;
							exitinggame = false;
							world->DropConnection();
						}	
						MsgID = 1;	
						World::MBHidden = true;
						this->BT_CharDeleteOK->MouseClickProccessed();
						CancelTrue = false;
					}
					if(this->BT_CharDeleteCancel->MouseClickedOnElement())
					{
						MsgID = 0;
						World::MBHidden = true;
						this->BT_CharDeleteCancel->MouseClickProccessed();
						CancelTrue = false;
					}

				}

			}
	}
	this->MouseWheelVal = 0;
}
long timerrecorder;
std::basic_string<wchar_t> istring;
int Game_FPSCounter = 0;
void Game::Render()
{
	Game_FPSCounter++;
    // Clear the render target and the zbuffer 
    Device->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_ARGB( 255, 0, 0, 0 ), 0.0f, 0 );
    // Render the scene
    if( SUCCEEDED( Device->BeginScene() ) )
    {
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		if (this->Stage <= 4)
		{
				this->menu->Render();
		}
		else
		{
			this->map->Render();
			Map_UserInterface->Render();
			this->map->ThreadLock.lock();
			RECT rct;

			rct.left = 20;
			rct.right = 300;
			rct.top = 32;
			rct.bottom = 300;

			this->map->Sprite->Begin(D3DXSPRITE_ALPHABLEND);
			this->DefaultFont->DrawTextW(this->map->Sprite, this->MapCursor.cur_istring.c_str(), -1, &rct, DT_LEFT, D3DCOLOR_ARGB(255, 255, 255, 255));
			this->map->Sprite->End();
			this->map->ThreadLock.unlock();
		}
		
		if(this->Stage > 1)
		{ this->BT_ExitGame->Draw(sprite); }
		world->W_UI_Infobox->Draw(sprite);
		if(!World::MBHidden)
		{
			this->Draw(sprite,this->MessageBoxTexture.Texture,this->MessageX,this->MessageY,D3DCOLOR_ARGB(255,255,255,255));
			if(!this->CancelTrue)
			{
			this->BT_Message_OK->Draw(sprite);
			}
			else
			{
				this->BT_CharDeleteOK->Draw(sprite);
				this->BT_CharDeleteCancel->Draw(sprite);
			}
			RECT rct;
			rct.left= this->MessageX+ 60;
			rct.right= rct.left + 270;
			rct.top= this->MessageY+ 25;
			rct.bottom= rct.top + 30;
			this->MessageFont->DrawTextA(sprite,World::MBTitle.c_str(),-1,&rct, DT_LEFT, D3DCOLOR_ARGB(255, 240, 240, 199));
			rct.left= this->MessageX+ 20;
			rct.right= rct.left + 270;
			rct.top= this->MessageY+ 60;
			rct.bottom= rct.top + 130;
			this->MessageFont->DrawTextA(sprite,World::MBMessage.c_str(),-1,&rct, DT_LEFT, D3DCOLOR_ARGB(255, 240, 240, 199));
		}	
		//Draw stuff the masks above others
	
		sprite->End();
        Device->EndScene();
    }


	timerrender = clock();
	if(timerrender - timerrecorder > 1000)
	{
	istring.clear();
	wchar_t clockstr[12];
	_itow_s(FPS,clockstr,7,10);
	istring += clockstr;
	istring += L" fps";
	
	timerrecorder = timerrender; 
	}
	this->DrawTextW(istring.c_str(),33,14,D3DCOLOR_ARGB( 205, 255, 255, 255 ));
	
	
	Device->Present( NULL, NULL, NULL, NULL );
	final = std::chrono::high_resolution_clock::now();
	//if (final - init > 0)
	{//this->FPS =
		std::chrono::duration<double> diff = final - init;
		this->FPS = 1 / diff.count();
	}

	
	//World::DebugPrint(std::to_string(FPS).c_str());
	if(FPS > 30)
	{

	}
}
void Game::Unload()
{
	if(this->DefaultFont){this->DefaultFont->Release();}
	this->DefaultFont = NULL;
	if(this->menu){this->menu->Release();}
	if(this->ResourceManager){this->ResourceManager->Release();}
	//this->ResourceManager = NULL;

	if(sprite){sprite->Release();}
	sprite = NULL;
	if(texture){texture.reset();}

	while(true)
	{
	}
}
void Game::ResetDevice()
{
		D3DXCreateSprite(Device,&sprite);
}

void Game::Draw(ID3DXSprite* Sprite,std::shared_ptr<IDirect3DTexture9> Texture, int x, int y, int Imgw, int Imgh, float Angle, D3DXCOLOR Color = D3DCOLOR_ARGB(255, 255, 255, 255))
{
		D3DXMATRIX mat;
		RECT SrcRect;
		SrcRect.left = x;
		SrcRect.top = y;
		SrcRect.bottom = y + Imgh;
		SrcRect.right = x + Imgw;
		
		// out, scaling centre, scaling rotation, scaling, rotation centre, rotation, translation
		D3DXVECTOR2 rotcentre;
		rotcentre.x = x + (Imgw / 2);
		rotcentre.y = y + (Imgh / 2);

		D3DXMatrixTransformation2D(&mat,NULL,NULL,NULL,&rotcentre, Angle  * (3.14 / 180),NULL);
		D3DXVECTOR3* Pos = new D3DXVECTOR3(x,y, 0.1f);
		D3DXVECTOR3* Center = new D3DXVECTOR3(1,1,0);
		Sprite->SetTransform(&mat);
		Sprite->Draw(Texture.get(),NULL,Center, Pos, Color);
}

void Game::DrawTextW(LPCWSTR str, int x, int y, D3DXCOLOR Color = D3DCOLOR_ARGB(255, 255, 255, 255))
{
	RECT rct;
	rct.left= x;
	rct.right= 640-x;
	rct.top= y;
	rct.bottom= 480-y;
	
	this->DefaultFont->DrawText(NULL, str, -1, &rct, 0, Color );
}
void Game::DrawText(LPCSTR str, int x, int y,  D3DXCOLOR Color)
{
	RECT rct;
	rct.left= x;
	rct.right= 100;
	rct.top= y;
	rct.bottom= 100;
	
	this->DefaultFont->DrawTextA(NULL, str, -1, &rct, 0, Color );
}
void Game::DrawTextW(ID3DXSprite* Sprite,LPCWSTR str, int x, int y,  D3DXCOLOR Color)
{
	RECT rct;
	rct.left= x;
	rct.right= 100;
	rct.top= y;
	rct.bottom= 100;
	
	this->DefaultFont->DrawText(Sprite, str, -1, &rct, 0, Color );
}
void Game::DrawText(ID3DXSprite* Sprite,LPCSTR str, int x, int y,  D3DXCOLOR Color)
{
	RECT rct;
	rct.left= x;
	rct.right= 100;
	rct.top= y;
	rct.bottom= 100;
	
	this->DefaultFont->DrawTextA(Sprite, str, -1, &rct, 0, Color );
}
void Game::Draw(ID3DXSprite* Sprite, std::shared_ptr<IDirect3DTexture9> Texture, int x, int y, D3DXCOLOR Color = D3DCOLOR_ARGB(255, 255, 255, 255))
{
		D3DXMATRIX mat;
		RECT SrcRect;
		SrcRect.left = x;
		SrcRect.top = y;
		SrcRect.bottom = 0;
		SrcRect.right = 0;
		
		D3DXMatrixTransformation2D(&mat,NULL,NULL,NULL,NULL, NULL,NULL);
		D3DXVECTOR3* Pos = new D3DXVECTOR3(x,y, 0.1f);
		D3DXVECTOR3* Center = new D3DXVECTOR3(1,1,0);
		Sprite->SetTransform(&mat);
		Sprite->Draw(Texture.get(),NULL,Center, Pos, Color);
		delete Pos;
		delete Center;
}
std::basic_string<wchar_t> Game_istring;

void Game::Draw(ID3DXSprite* Sprite, std::shared_ptr<IDirect3DTexture9> Texture, int x, int y, float depth, D3DXCOLOR Color = D3DCOLOR_ARGB(255, 255, 255, 255))
{
	D3DXVECTOR3* Pos = new D3DXVECTOR3(x, y, depth);
	D3DXVECTOR3* Center = new D3DXVECTOR3(1, 1, 0);
	//Color.a = depth;
	Sprite->Draw(Texture.get(), NULL, Center, Pos, Color);
	delete Pos;
	delete Center;
}

void Game::Draw(ID3DXSprite* Sprite, IDirect3DTexture9* Texture, int x, int y, D3DXCOLOR Color = D3DCOLOR_ARGB(255, 255, 255, 255))
{
		D3DXMATRIX mat;
		RECT SrcRect;
		SrcRect.left = x;
		SrcRect.top = y;
		SrcRect.bottom = 0;
		SrcRect.right = 0;
		
		D3DXMatrixTransformation2D(&mat,NULL,NULL,NULL,NULL, NULL,NULL);
		D3DXVECTOR3* Pos = new D3DXVECTOR3(x,y, 0.1f);
		D3DXVECTOR3* Center = new D3DXVECTOR3(1,1,0 );
		Sprite->SetTransform(&mat);
		Sprite->Draw(Texture,NULL,Center, Pos, Color);
		
		delete Pos;
		delete Center;
}
void Game::SetStage(GameStage _Stage)
{
	this->Stage = _Stage;
}

Game::~Game()
{
	this->Map_UserInterface->~Map_UI();
}