
#ifdef __cplusplus
extern "C" {
#endif

#define bar_dimensiune 20

#define width_nr 800
#define width width_nr-bar_dimensiune
#define height 600

#define fps 16
#define cell 20

#define bar_x width
#define bar_y 0
#define bar_w bar_dimensiune
#define bar_h height

void declare_grid(int grid_width,int grid_height,char* grid,char* init_value);
void debug_grid(char* container,int grid_width,int grid_height,char* grid,int dist,int color);

void shared_write(); void shared_write_ex(bool circle, int fuel, int rotation, bool space_env, bool keys);

int presprite;int sprite;
int bmp;//in ep 10 si altele
int dbl;

#define buf_size 131072
char buf[buf_size];
char buf2[buf_size];//la list la dinbufinbuf
#define information "informer"

typedef struct key__struct
{
    char* name;
    char* defaultKey;
    char* disp_name;
}key_struct;
key_struct left_arrow={"left_arrow","LEFT","Left Direction"};
key_struct right_arrow={"right_arrow","RIGHT","Right Direction"};
key_struct up_arrow={"up_arrow","UP","Up Direction"};
key_struct down_arrow={"down_arrow","DOWN","Down Direction"};
key_struct action_key={"action_key","CONTROL","Primary Action"};
key_struct jump_aux_key={"jump_aux_key","SPACE","Jump/Secondary Action"};
key_struct strafe_left={"strafe_left","Z","Strafe Left"};
key_struct strafe_right={"strafe_right","X","Strafe Right"};
key_struct auto_shot={"auto_shot","A","Auto-Shot"};
key_struct auto_move={"auto_move","S","Auto-Move"};
char* mouse_disp_name = "Mouse Press";

key_struct* keyboards[]={&left_arrow,&right_arrow,&up_arrow,&down_arrow,&action_key,&jump_aux_key,&strafe_left,&strafe_right,&auto_shot,&auto_move};
int keyboards_total=sizeof(keyboards)/sizeof(key_struct*);

typedef struct ep__keys{
    key_struct* key;
    char* key_desc;
	char* key_alt;
}ep_keys;
typedef struct episooade_struct
{
    char* name;
    char* description;
    ep_keys* episod_chei;
    char*mousePress;
	int id_stat;
}episoade_pack;
//left_arrow.disp_name nu vrea
#define Ores_for_Ship_ep "Ores_for_Ship"
ep_keys Ores_for_Ship_keys[]={{&left_arrow,"Movement","Left Direction"},{&right_arrow,"Movement","Right Direction"},{&up_arrow,"Movement","Up Direction"},{&down_arrow,"Movement","Down Direction"},{NULL}};
episoade_pack Ores_for_Ship={Ores_for_Ship_ep,"Gather all the required minerals.",Ores_for_Ship_keys,NULL,128541};
//
#define Driving_the_Ship_ep "Driving_the_Ship"
ep_keys Driving_the_Ship_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{&up_arrow,"Movement","Up Direction" },{&down_arrow,"Movement","Down Direction" },{NULL}};
episoade_pack Driving_the_Ship = { Driving_the_Ship_ep,"Pass through the blue clouds. Bonuses: Green = cloud to ship; Blue = free taker; Yellow = cable taker; Red = shield.",Driving_the_Ship_keys,NULL,128542 };
//
#define Planet_Landing_ep "Planet_Landing"
ep_keys Planet_Landing_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{&down_arrow,"Movement","Down Direction" },{NULL}};
episoade_pack Planet_Landing={Planet_Landing_ep,"Pass through the obstacles and land on the planet.",Planet_Landing_keys,NULL,128543};
//
#define Rocks_Fall_ep "Rocks_Fall"
ep_keys Rocks_Fall_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{&up_arrow,"Move gun","Gun Up Direction"},{&down_arrow,"Move gun","Gun Down Direction"},{&action_key,"Shot","Shot"},{&jump_aux_key,"Reset gun position","Reset gun position" },{ &auto_shot,"Continuous shot","Auto-Shot" },{ &auto_move,"Continuous right","Auto-Move" },{NULL}};
episoade_pack Rocks_Fall={Rocks_Fall_ep,"Pass through the obstacles.",Rocks_Fall_keys,NULL,128544};
//
#define Balls_Play_ep "Balls_Play"
ep_keys Balls_Play_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{&action_key,"Launch circle","Launch circle" },{&jump_aux_key,"Move slower (pressing)","Move slower (pressing)" },{NULL}};
episoade_pack Balls_Play={Balls_Play_ep,"The field circles must hit the galaxy. The field circles must not hit the defender and must hit the line behind the defender.",Balls_Play_keys,NULL,128545};
//
#define Asteroids_ep "Asteroids"
ep_keys Asteroids_keys[]={{&left_arrow,"Movement","Rotate Left" },{&right_arrow,"Movement","Rotate Right" },{&up_arrow,"Movement","Forward" },{&down_arrow,"Movement","Backward" },{&action_key,"Shot","Shot"},{ &auto_shot,"Continuous shot","Auto-Shot" },{NULL}};
episoade_pack Asteroids={Asteroids_ep,"Shot the asteroids. There are four types of bonuses and a progress bar.",Asteroids_keys,NULL,128546};
//
ep_keys Cubes_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{&down_arrow,"Cube fall","Cube fall" },{NULL}};
episoade_pack Cubes={"Cubes","Match the cubes. A match is formed by three cubes or more placed one after another horizontally or vertically. Hold Left/Right to move faster.",Cubes_keys,NULL,128548};
//
#define Moving_Forward_ep "Moving_Forward"
ep_keys Moving_Forward_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{NULL}};
episoade_pack Moving_Forward={Moving_Forward_ep,"Pass through the obstacles.",Moving_Forward_keys,NULL,128547};
//
#define Jump_ep "Jump"
ep_keys Jump_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{&jump_aux_key,"Jump","Jump" },{NULL}};
episoade_pack Jump={Jump_ep,"Reach the top of the zones.",Jump_keys,NULL,128549};
//
#define On_the_Rail_ep "On_the_Rail"
ep_keys On_the_Rail_keys[]={{&left_arrow,"Rotation","Rotate Left" },{&right_arrow,"Rotation","Rotate Right"},{&action_key,"Shot","Shot" },{&jump_aux_key,"Move faster (pressing)","Move faster (pressing)" },{ &auto_shot,"Continuous shot","Auto-Shot" },{NULL}};
episoade_pack On_the_Rail={On_the_Rail_ep,"Shot all the targets.",On_the_Rail_keys,NULL,128550};
//
#define Space_Trip_ep "Space_Trip"
ep_keys Space_Trip_keys[]={{&up_arrow,"Movement","Up Direction"},{&down_arrow,"Movement","Down Direction"},{&action_key,"Shot","Shot" },{ &auto_shot,"Continuous shot","Auto-Shot" },{NULL}};
episoade_pack Space_Trip={Space_Trip_ep,"Pass through the obstacles.",Space_Trip_keys,NULL,128551};
//
#define Space_Zones_ep "Space_Zones"
ep_keys Space_Zones_keys[]={{&left_arrow,"Rotation","Rotate Left"},{&right_arrow,"Rotation","Rotate Right"},{&up_arrow,"Movement","Forward"},{&down_arrow,"Movement","Backward"},{&action_key,"Shot","Shot" },{&jump_aux_key,"Rotate slower (pressing)","Rotate slower (pressing)" },{ &auto_shot,"Continuous shot","Auto-Shot" },{NULL}};
episoade_pack Space_Zones={Space_Zones_ep,"Reach the end platforms, dispose of the enemies, shot all the targets or avoid the enemies.",Space_Zones_keys,NULL,128552};
//
#define Rooms_ep "Rooms"
ep_keys Rooms_keys[]={{&left_arrow,"Rotation","Rotate Left"},{&right_arrow,"Rotation","Rotate Right"},{&up_arrow,"Movement","Forward"},{&down_arrow,"Movement","Backward"},{&strafe_left,"Movement","Strafe Left"},{&strafe_right,"Movement","Strafe Right"},{&action_key,"Shot","Shot" },{ &auto_shot,"Continuous shot","Auto-Shot" },{ &auto_move,"Continuous backward","Auto-Backward" },{NULL}};
episoade_pack Rooms={Rooms_ep,"Shot all the ships.",Rooms_keys,NULL,128553};
//
#define Road_ep "Road"
ep_keys Road_keys[]={{&left_arrow,"Rotation","Rotate Left"},{&right_arrow,"Rotation","Rotate Right"},{&action_key,"Shot","Shot" },{&strafe_left,"Movement","Strafe Left"},{&strafe_right,"Movement","Strafe Right"},{ &auto_shot,"Continuous shot","Auto-Shot" },{NULL}};
episoade_pack Road={Road_ep,"Follow the road until the last zone.",Road_keys,NULL,128554};
//
episoade_pack Racecourse={"Racecourse","A chance-dependent game with running horses. Choose a horse at start or wait them all.",NULL,"Choose Horse",128511};
episoade_pack Card_Flip={"Card_Flip","Flip the cards and find the same ones.",NULL,"Choose Card",128512};
episoade_pack Treasure_Finder={"Treasure_Finder","Find all the treasures under the sand.",NULL,"Dig Area",128513};
episoade_pack Fruit_Cocktail={"Fruit_Cocktail","Fruit Cocktail is a puzzle game where it is required to put fruits on the table to make the cocktail.",NULL,"Place/Remove Fruit",	129353 };
episoade_pack Naval_War= { "Naval_War","Naval War is a strategy game. The scope is to destroy the computer ships as fast as possible.",NULL,"Place/Target Ships",129481 };
episoade_pack Turret_Defence = { "Turret_Defence","Turret Defence is a strategy game. The scope is to destroy an incoming enemy.",NULL,"Take/Place Turrets",129748 };
episoade_pack Ball_Hit= { "Ball_Hit","Hit the ball with the baseball bat at the best angle.",NULL,"Hit",129933};
episoade_pack Lotto= { "Lotto","Lotto is a chance game with 5 numbers selected from 30.",NULL,"Select Ball",130064};
episoade_pack Roulette= { "Roulette","Roulette is a casino game. The luck plays an important role to obtain a good score.",NULL,"Start Power/Spin",130137 };
//
episoade_pack Mahjong = { "Mahjong","Mahjong is a puzzle game. Match all the tiles.",NULL,"Select Tile",131988 };
//
episoade_pack* episoade[]={&Ores_for_Ship,&Driving_the_Ship,&Planet_Landing,&Rocks_Fall,&Balls_Play,&Asteroids,&Cubes,&Moving_Forward,&Jump,&On_the_Rail,&Space_Trip,&Space_Zones,&Rooms,&Road,&Racecourse,&Card_Flip,&Treasure_Finder,&Fruit_Cocktail,&Naval_War,&Turret_Defence,&Ball_Hit,&Lotto,&Roulette,&Mahjong };
int numarulEpisoadelor_total=sizeof(episoade)/sizeof(episoade_pack*);
#define lista_ferestre_pagina 12
#define episoade_pagina lista_ferestre_pagina-1

int get_ep_nr(ep_keys* ep){
    int i=0;
    for(;;){
        int* relevant_pointer=(int*)&ep[i];
        //de 0 ca am pus un {NULL}
        if(relevant_pointer[0]==0)return i;
        i++;
    }
}
char prepare_def[100];
char* get_def_code(char* key_def){
    if(strlen(key_def)!=1)sprintf(prepare_def,"Key.%s",key_def);
    else sprintf(prepare_def,"ord('%s')",key_def);
    return prepare_def;
}
void set_init(ep_keys* ep_chei){
	//if (ep_chei == NULL)return;
    action("var com=_root['shared_level']");
    int key_nr=get_ep_nr(ep_chei);
    for(int i=0;i<key_nr;i++){
        char* key_id=ep_chei[i].key->name;
        actionf(buf,"if(com.%s==undefined)com.%s=%s",key_id,key_id,get_def_code(ep_chei[i].key->defaultKey));
    }
}
/*void securitate(){
    action(R"(
        //asta trebuie cand e de la //www la //
        //fapt divers: kongregate.com devine www.kongregate.com
        System.security.allowDomain(_parent._url);
        //System.security.allowDomain('www.universe-game.com');
        //if(_root.kongregate!=undefined)System.security.allowDomain('chat.kongregate.com');
    )");
}*/
int find_ep(char*ep) {
	int ep_nr = 0; int sz = strlen(ep);
	while (strcmp(episoade[ep_nr]->name, ep) != 0){
		ep_nr++;
	}
	return ep_nr;
}
void init_episod(char* ep){
	ep_keys*k=episoade[find_ep(ep)]->episod_chei;
    set_init(k);
}

#define static_text_off_subtract 10
#define list_lineSz 2

#define sorts_length 3
#define rat "Rating"
#define pop "Plays"
char* sorts[sorts_length]={"Chronology",pop,rat};
//da null asa, oare de ce: char* pop = sorts[1]; char* rat = sorts[2];
//The compiler will automatically concatenate adjacent strings
//#define rat_user rat "_user"
#define scor_bazic_string "'0'"

void din_buf_in_buf(char* s, ...) {
	va_list arglist;
	va_start(arglist, s);
	vsprintf(buf2, s, arglist);
	va_end(arglist);
	strcat(buf, buf2);
	//sprintf(buf2, "%s", buf);
	//sprintf(buf, "%s%s", buf2, buf3);
}

void automatic(int n,char* extra_move=NULL){
	bool shot = (n & 1) != 0; bool move = (n & 2) != 0;
	//asta daca era cu toate, era continua, asa e doar odata
	if (shot)action("var auto_shot=false");
	if (move)action("var auto_move=false");
	sprintf(buf, "var listener_automatic=new Object();listener_automatic.onKeyDown=function(){");
	if (shot)din_buf_in_buf("if(Key.getCode()==com.auto_shot)auto_shot=auto_shot==false?true:false;");
	if (move) {
		if (shot)din_buf_in_buf("else ");
		din_buf_in_buf("if(Key.getCode()==com.auto_move){auto_move=auto_move==false?true:false;");
		if (extra_move != NULL)din_buf_in_buf(extra_move);
		din_buf_in_buf("}");
	}
	din_buf_in_buf("}"); action(buf);
	action(R"(
	Key.addListener(listener_automatic);
	//nu merge cu onUnload cand se da Home Back la Universe, comportament ciudat
	_root.automatics_func=function(){
//e testat, asta se executa bine
//_root.createTextField('qwe',_root.getNextHighestDepth(),0,0,100,100);
//_root.qwe.text=Key._listeners.length+' ';
		Key.removeListener(listener_automatic);
//_root.qwe.text+=Key._listeners.length+' ';
	}
	//onUnload=function(){
		//Key.removeListener(listener);
	//}
	)");
}

#define shot_interval fps/3

#ifdef __cplusplus
}
#endif
