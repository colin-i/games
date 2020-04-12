


format elfobj64microsoft

warning off
include "../actionswf/dev/import.h"
warning on
include "../actionswf/include/prog.h"

const text_height=30
const stock_text_height=15
const cocktailsize_half=10
const cocktailsize=2*cocktailsize_half
const fps=12

chars Apple="Apple"
chars *="Banana"
chars *="Grape"
chars *="Kiwi"
chars *="Orange"
chars *="Pineapple"
chars *="Strawberry"
chars *=0

str fruits^Apple

include "../files/tool.s"
include "../files/submit.s"

str game_name="Fruit Cocktail"

function set_stock(ss name,sd font,sd x,sd y,sd index)
    sd presprite
    sd sprite

    chars spc_data#500
    str spc^spc_data

    setcall presprite swf_sprite_new()

    #bitmap
    str fruit="D:/universe/local/fruitcocktail/%s.dbl"
    call sprintf(spc,fruit,name)
    data width#1
    data height#1
    sd dbl_id
    sd shape_id
    setcall dbl_id swf_dbl_ex(spc,#width)
    setcall shape_id swf_shape_bitmap(dbl_id,width,height)
    call swf_sprite_placeobject(presprite,shape_id,1)

    #text
    sd txt
    setcall txt static_text(width,name,font,(stock_text_height))
    call swf_sprite_placeobject_coords(presprite,txt,2,0,height)

    call sprintf(spc,"_x=%u;_y=%u;var current_border;
    function stock_press()_root.item_on_table(_name);
    ",x,y)
    call action_sprite(presprite,spc)

    #black
    call action_sprite(presprite,"current_border=_root.the_border(this,'black');stop()")
    call swf_sprite_showframe(presprite)
    #white
    call action_sprite(presprite,"current_border=_root.add_border(this,'white',current_border)")
    call swf_sprite_showframe(presprite)
    #blue
    call action_sprite(presprite,"current_border=_root.add_border(this,'blue',current_border)")
    call swf_sprite_showframe(presprite)
    #red
    call action_sprite(presprite,"current_border=_root.add_border(this,'red',current_border)")
    call swf_sprite_showframe(presprite)
    #green
    call action_sprite(presprite,"current_border=_root.add_border(this,'green',current_border)")
    call swf_sprite_showframe(presprite)
    #
    setcall sprite swf_sprite_done(presprite)
    call swf_exports_add(sprite,name)
    call actionf(spc,"attachMovie('%s','%s',getNextHighestDepth())",name,name)

    #
    chars nm#100
    str item^nm
    call sprintf(item,"item%s",name)
    call sprintf(spc,fruit,item)

    setcall presprite swf_sprite_new()
    setcall dbl_id swf_dbl_ex(spc,#width)
    setcall shape_id swf_shape_bitmap(dbl_id,width,height)
    call swf_sprite_placeobject(presprite,shape_id,1)
    mult height -1
    call actionf_sprite(presprite,spc,"_y=%u;var fruit_index=%u",height,index)
    call action_sprite(presprite,"_root.fruits_counter[fruit_index]++;var pos=_root.just_pressed_nr;
        onPress=function()_root.table_item_out(this)")
    call swf_sprite_showframe(presprite)
    setcall sprite swf_sprite_done(presprite)
    call swf_exports_add(sprite,item)
endfunction

entry fruitcocktail()

str env_path="D:/universe/local/fruitcocktail/env.dbl"
str Apple_path="D:/universe/local/fruitcocktail/Apple.dbl"
str Apple_item_path="D:/universe/local/fruitcocktail/itemApple.dbl"
str glass_path="D:/universe/local/fruitcocktail/glass.dbl"
str full_glass_path="D:/universe/local/fruitcocktail/full_glass.dbl"

sd env_w
sd env_h

sd fruits_total=0

chars a_code#16384
str ac^a_code

setcall env_w swf_dbl_width(env_path)
setcall env_h swf_dbl_height(env_path)

sd stock_w
sd stockitem_h
setcall stock_w swf_dbl_width(Apple_path)
setcall stockitem_h swf_dbl_height(Apple_path)
sd stock_h
set stock_h stockitem_h
add stock_h (stock_text_height+10)

ss p
sd i
sd shape

set p fruits
while p#!=0
    inc fruits_total
    addcall p strlen(p)
    inc p
endwhile

sd all_stock_w
#sd all_stock_h
set all_stock_w stock_w
mult all_stock_w fruits_total
sd free_w
set free_w env_w
sub free_w all_stock_w
sd dv=1
add dv fruits_total
div free_w dv

sd item_w
sd item_h
setcall item_w swf_dbl_width(Apple_item_path)
setcall item_h swf_dbl_height(Apple_item_path)
sd glass_w
sd glass_h
setcall glass_w swf_dbl_width(glass_path)
setcall glass_h swf_dbl_height(glass_path)

#

call sprintf(ac,"%s.swf",game_name)
call swf_new(ac,env_w,env_h,0x112233,(fps))

sd deffont
setcall deffont swf_font_basic("_sans")

sd presprite
sd img
sd sprite

#env|
setcall presprite swf_sprite_new()
setcall img swf_image(env_path)
call swf_sprite_placeobject(presprite,img,1)
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"env")
call action("attachMovie('env','env',1)")
#|env

#stocks|
    #stock borders
    #black
    setcall presprite swf_sprite_new()
    setcall shape swf_shape_border(stock_w,stock_h,1,0x010101FF)
    call swf_sprite_placeobject(presprite,shape,1)
    setcall sprite swf_sprite_done(presprite)
    call swf_exports_add(sprite,"black")
    #white
    setcall presprite swf_sprite_new()
    setcall shape swf_shape_border(stock_w,stock_h,1,0xffFFffFF)
    call swf_sprite_placeobject(presprite,shape,1)
    setcall sprite swf_sprite_done(presprite)
    call swf_exports_add(sprite,"white")
    #blue
    setcall presprite swf_sprite_new()
    setcall shape swf_shape_border(stock_w,stock_h,1,0x0101ffFF)
    call swf_sprite_placeobject(presprite,shape,1)
    setcall sprite swf_sprite_done(presprite)
    call swf_exports_add(sprite,"blue")
    #red
    setcall presprite swf_sprite_new()
    setcall shape swf_shape_border(stock_w,stock_h,1,0xff0101FF)
    call swf_sprite_placeobject(presprite,shape,1)
    setcall sprite swf_sprite_done(presprite)
    call swf_exports_add(sprite,"red")
    #green
    setcall presprite swf_sprite_new()
    setcall shape swf_shape_border(stock_w,stock_h,1,0x01ff01FF)
    call swf_sprite_placeobject(presprite,shape,1)
    setcall sprite swf_sprite_done(presprite)
    call swf_exports_add(sprite,"green")
    #
call action("
//border_movie
function add_border(item,name,prev_border)
{
    prev_border.removeMovieClip();
    var current_border=the_border(item,name);
    return current_border;
}
//border_movie
function the_border(item,name)
{
    item.attachMovie(name,name,item.getNextHighestDepth());
    return item[name];
}
")
    #
sd x
sd y
set y env_h
sub y 20
sub y stock_h
set i 0
set p fruits
while p#!=0
    set x i
    mult x stock_w
    sd free=1
    add free i
    mult free free_w
    add x free
    call set_stock(p,deffont,x,y,i)
    addcall p strlen(p)
    inc p
    inc i
endwhile
#|stocks

#table zone|
sd land_y
set land_y y
sub land_y 20
sd table_y
setcall presprite swf_sprite_new()
#
data tablepart_w#1
data tablepart_h#1
sd dbl_id
setcall dbl_id swf_dbl_ex("D:/universe/local/fruitcocktail/tablepart.dbl",#tablepart_w)
#
data umbrella_w#1
data umbrella_h#1
sd dbl_umbrella_id
setcall dbl_umbrella_id swf_dbl_ex("D:/universe/local/fruitcocktail/umbrella1.dbl",#umbrella_w)
#
sd table_w=cocktailsize
sd table_top_h=2
mult table_w item_w
add table_w glass_w
set table_y land_y
sub table_y tablepart_h
sd table_depths=1
sd free_add
#umbrella
sd umbrella1_id
setcall umbrella1_id swf_shape_bitmap(dbl_umbrella_id,umbrella_w,umbrella_h)
sd umbrella2_id
setcall umbrella2_id swf_image("D:/universe/local/fruitcocktail/umbrella2.dbl")
sd umbrella_y=-1
mult umbrella_y umbrella_h
add umbrella_y tablepart_h
set i 4
sd u_free_w
sd u_all_free_w
sd all_umbrella_w
set all_umbrella_w umbrella_w
mult all_umbrella_w i
set u_all_free_w table_w
sub u_all_free_w all_umbrella_w
sd u_div=1
add u_div i
set u_free_w u_all_free_w
div u_free_w u_div
sd u_x
while i>0
    dec i
    set u_x i
    mult u_x umbrella_w
    set free_add 1
    add free_add i
    mult free_add u_free_w
    add u_x free_add
    sd and=1
    and and i
    sd id
    if and==0
        set id umbrella1_id
    else
        set id umbrella2_id
    endelse
    call swf_sprite_placeobject_coords(presprite,id,table_depths,u_x,umbrella_y)
    inc table_depths
endwhile
#table parts
sd tablepart_id
setcall tablepart_id swf_shape_bitmap(dbl_id,tablepart_w,tablepart_h)
#
set i 6
sd t_free_w
sd t_all_free_w
sd all_tablepart_w
set all_tablepart_w tablepart_w
mult all_tablepart_w i
set t_all_free_w table_w
sub t_all_free_w all_tablepart_w
sd t_div=1
add t_div i
set t_free_w t_all_free_w
div t_free_w t_div
sd t_x
while i>0
    dec i
    set t_x i
    mult t_x tablepart_w
    set free_add 1
    add free_add i
    mult free_add t_free_w
    add t_x free_add
    call swf_sprite_placeobject_coords(presprite,tablepart_id,table_depths,t_x,0)
    inc table_depths
endwhile
#table top
sd table
data border=no_fill
data h#1
set h table_top_h
data *=0xDD9922ff
data *={0,StateLineStyle}
data *={1,1}
data w#1;data *=0
set w table_w
data *=-1
setcall table swf_shape(table_w,table_top_h,#border)
call swf_sprite_placeobject(presprite,table,table_depths)
inc table_depths
#glass
sd glasspre
sd glass_sprite
sd glass_bm_id
sd glass_x
set glass_x table_w
sub glass_x glass_w
div glass_x 2
    #
setcall glasspre swf_sprite_new()
setcall glass_bm_id swf_dbl(glass_path)
setcall shape swf_shape_bitmap(glass_bm_id,glass_w,glass_h)
call swf_sprite_placeobject(glasspre,shape,1)
call actionf_sprite(glasspre,ac,"_x=%u;_y=0-_height;
    onPress=_root.glass_press",glass_x)
call swf_sprite_showframe(glasspre)
setcall glass_sprite swf_sprite_done(glasspre)
call swf_exports_add(glass_sprite,"glass")
call action_sprite(presprite,"attachMovie('glass','glass',getNextHighestDepth());glass._visible=false")
    #
data full_glass_w#1
data full_glass_h#1
setcall glasspre swf_sprite_new()
setcall glass_bm_id swf_dbl_ex(full_glass_path,#full_glass_w)
setcall shape swf_shape_bitmap(glass_bm_id,full_glass_w,full_glass_h)
call swf_sprite_placeobject(glasspre,shape,2)
call actionf_sprite(glasspre,ac,"_x=%u;_y=0-_height",glass_x)
call swf_sprite_showframe(glasspre)
setcall glass_sprite swf_sprite_done(glasspre)
call swf_exports_add(glass_sprite,"full_glass")
    #
call action_sprite(presprite,"attachMovie('full_glass','full_glass',getNextHighestDepth())")
#
sd table_x
set table_x env_w
sub table_x table_w
div table_x 2
call actionf_sprite(presprite,ac,"_x=%u;_y=%u",table_x,table_y)
#
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"table")
call action("attachMovie('table','table',getNextHighestDepth())")
#|table zone

#texter|
sd text_w
sd text_x=15
set text_w env_w
sub text_w 30
setcall presprite swf_sprite_new()
call actionf_sprite(presprite,ac,"_x=%u;_y=15",text_x)
setcall shape swf_shape_basic(text_w,(text_height+15),0xffFFffFF,0x0101FF66)
call swf_sprite_placeobject(presprite,shape,1)
sd txt
ss texter
setcall texter texter_name()
ss text_msg
setcall text_msg texter_msg()
setcall txt dyn_text(text_w,text_msg,deffont,(text_height))
call swf_sprite_placeobject_coords(presprite,txt,2,20,0)
call swf_sprite_showframe(presprite)
#
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,texter)
call actionf(ac,"attachMovie('%s','%s',getNextHighestDepth())",texter,texter)
#|texter

call sprintf(ac,"var fruits=new Array(")
set p fruits
while p#!=0
    call strcat(ac,"'")
    call strcat(ac,p)
    call strcat(ac,"',")
    addcall p strlen(p)
    inc p
endwhile
ss cursor
set cursor ac
addcall cursor strlen(ac)
dec cursor
chars fruits_group_end=")"
set cursor# fruits_group_end
call action(ac)

sd max_random
set max_random (cocktailsize)
div max_random fruits_total
call actionf(ac,"var max_random=%u;var cocktail_size=%u",max_random,(cocktailsize))
call actionf(ac,"var cocktail_size_half=%u",(cocktailsize_half))

call action("
var fruits_values=new Array();var table_fruits=new Array();var fruits_counter=new Array();
var score;
function at_start()
{
    _text.message='Click to play';
    onMouseDown=is_starting;
}
function dummy_function(){}
function is_starting()
{
    onMouseDown=dummy_function;
    newgame();
}
function newgame()
{
    score=0;
    _text.message='Ready';
    table.full_glass._visible=false;
    table.glass._visible=true;

    fruits_values.length=0;var fruits_on_table=0;var nr;
    i=0;while(i<fruits.length){nr=random(max_random);nr++;fruits_on_table=fruits_on_table+nr;
        fruits_values.push(nr);i=i+1;}
    while(fruits_on_table<cocktail_size){
        fruits_values[random(fruits.length)]++;fruits_on_table++;}

    var i=0;while(i<fruits.length){var stock=this[fruits[i]];
        stock.gotoAndStop(2);
        stock.onPress=stock.stock_press;
        stock.useHandCursor=true;
        i=i+1;}

    table_fruits.length=0;i=0;
    while(i<cocktail_size){table_fruits.push(0);i=i+1;}
    fruits_counter.length=0;i=0;
    while(i<fruits.length){fruits_counter.push(0);i=i+1;}
}
var just_pressed_nr;
function item_on_table(name)
{
    var i=0;
    while(i<table_fruits.length){
        if(table_fruits[i]==0){
            score++;
            _text.message=score;
            just_pressed_nr=i;
            var exp_name='item'+name;var instance_name=exp_name+i;table.attachMovie(exp_name,instance_name,table.getNextHighestDepth());
            var instance=table[instance_name];
            var x=i*instance._width;
            if(i>=cocktail_size_half)x=x+table.glass._width;
            instance._x=x;table_fruits[i]=instance;
            return undefined;
        }
        i=i+1;
    }
    _text.message='Press on the glass';
    return undefined;
}
var cocktail_test=new Array();
function glass_press()
{
    cocktail_test.length=0;
    var i=0;while(i<fruits.length){cocktail_test.push(0);i=i+1;}
    i=0;
    while(i<table_fruits.length){
        if(table_fruits[i]==0){_text.message='More fruits required';return undefined;}
        cocktail_test[table_fruits[i].fruit_index]++;i++;
    }
    i=0;var fruits_not_ok=0;
    while(i<fruits.length){
        var name=fruits[i];var inst=_root[name];
        if(fruits_not_ok==0)
        {
            var nr=cocktail_test[i];
            if(nr==fruits_values[i])inst.gotoAndStop(5);
            else if(nr<fruits_values[i]){
                _text.message='More fruits required: '+name;inst.gotoAndStop(3);fruits_not_ok=1;}
            else{
                _text.message='Too many fruits: '+name;inst.gotoAndStop(4);fruits_not_ok=1;}
        }
        else inst.gotoAndStop(2);
        i++;
    }
    if(fruits_not_ok==0){
        i=0;while(i<cocktail_size){table_fruits[i].removeMovieClip();i++;}
        i=0;while(i<fruits.length){var st=_root[fruits[i]];
            st.onPress=function(){};
            st.useHandCursor=false;
            i++;}
        table.glass._visible=false;
        table.full_glass._visible=true;
        submitform(score);
    }
}
function table_item_out(item)
{
    fruits_counter[item.fruit_index]--;
    table_fruits[item.pos]=0;
    item.removeMovieClip();
}
at_start();
")

call submit_form(deffont,env_w,env_h,game_name)

call swf_exports_done()
call swf_showframe()
call swf_done()
return 1
