

format elfobj64microsoft

warning off
include "../actionswf/dev/import.h"
warning on
include "../actionswf/include/prog.h"

const text_height=35

include "../files/tool.s"
include "../files/submit.s"

str game_name="Card Flip"

const pairs=18
const cards_nr=pairs*2
const cards_width=6
const cards_height=cards_nr/cards_width
const fps=12

function set_card(ss name,ss array_str,ss path)
    sd presprite
    sd img
    sd sprite

    setcall presprite swf_sprite_new()
    setcall img swf_image(path)
    call swf_sprite_placeobject(presprite,img,1)
    chars txt#100
    call sprintf(#txt,"var over=0;var object='%s';stop()",name)
    call action_sprite(presprite,#txt)

    sd frames=fps
    while frames>0
        call swf_sprite_showframe(presprite)
        dec frames
    endwhile
    call action_sprite(presprite,"
        var back=_root['b'+_name.substr(1)];
        if(over>0){
            back.removeMovieClip();
            if(over==2)_root.submitform(_root.moves);
            removeMovieClip();
        }
        back._visible=true;back.play()")
    call swf_sprite_showframe(presprite)
    setcall sprite swf_sprite_done(presprite)
    call swf_exports_add(sprite,name)

    call strcat(array_str,"'")
    call strcat(array_str,name)
    call strcat(array_str,"',")
endfunction

entry cardflip()

str env_path="D:/universe/local/cardflip/env.dbl"
str back_path="D:/universe/local/cardflip/back.dbl"

sd env_w
sd env_h

setcall env_w swf_dbl_width(env_path)
sd height
setcall height swf_dbl_height(env_path)
sd card_w
setcall card_w swf_dbl_width(back_path)
sd card_h
setcall card_h swf_dbl_height(back_path)
set env_h height
sub env_h (text_height)

chars a_code#16384
str ac^a_code
#D:/oapp/games/
call sprintf(ac,"%s.swf",game_name)

call swf_new(ac,env_w,height,0x112233,(fps))

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
call action("attachMovie('env','_env',1);")
#|env

sd deffont
setcall deffont swf_font_basic("_sans")
call default_text_withHeight(2,0xFF001100,deffont,env_h,(text_height))

#back|
setcall presprite swf_sprite_new()
setcall img swf_image(back_path)
call swf_sprite_placeobject(presprite,img,1)
call action_sprite(presprite,"
function card_press()
{
    _visible=false;var turn_card=_root['c'+_name.substr(1)];var prev_card=_root.prev_card;
    if(prev_card==0){_root.prev_card=turn_card;return undefined;}
    _root.prev_card=0;
    if(_root.moves<1000)_root.moves=_root.moves+1;
    _root._text.message=_root.moves;
    if(prev_card.object==turn_card.object)
    {
       prev_card.over=1;
       turn_card.over=1;
       _root.pairs=_root.pairs-1;
       if(_root.pairs==0)turn_card.over=2;
    }
    prev_card.play();
    turn_card.play();
}
onPress=card_press;
stop();
")
call swf_sprite_showframe(presprite)
sd shape
sd neg_w=-1
sd neg_h=-1
mult neg_w card_w
mult neg_h card_h
data border={no_fill,1,0xFF0000ff}
data *={0,StateLineStyle}
data *={1,1,0}
data est#1
set est card_w
data *={1,1,1}
data sud#1
set sud card_h
data *={1,1,0}
data west#1
set west neg_w
data *={1,1,1}
data nord#1
set nord neg_h
data *=-1
setcall shape swf_shape(card_w,card_h,#border)
call swf_sprite_placeobject(presprite,shape,2)
sd i=0
while i<(fps)
    call swf_sprite_showframe(presprite)
    inc i
endwhile
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"back")
#|back

#cards|
chars set_the_cards="var cards=new Array("
chars *#200
str the_cards^set_the_cards
call set_card("circle",the_cards,"D:/universe/local/cardflip/circle.dbl")
call set_card("club",the_cards,"D:/universe/local/cardflip/club.dbl")
call set_card("diamond",the_cards,"D:/universe/local/cardflip/diamond.dbl")
call set_card("flake",the_cards,"D:/universe/local/cardflip/flake.dbl")
call set_card("half-moon",the_cards,"D:/universe/local/cardflip/half-moon.dbl")
call set_card("heart",the_cards,"D:/universe/local/cardflip/heart.dbl")
call set_card("hexagon",the_cards,"D:/universe/local/cardflip/hexagon.dbl")
call set_card("rainbow",the_cards,"D:/universe/local/cardflip/rainbow.dbl")
call set_card("spade",the_cards,"D:/universe/local/cardflip/spade.dbl")
ss cursor
set cursor the_cards
addcall cursor strlen(the_cards)
dec cursor
set cursor# (Closeparenthesis)
call action(the_cards)
#|cards

call sprintf(ac,"var card_w=%u;var card_h=%u;var cards_width=%u;var cards_height=%u
",card_w,card_h,(cards_width),(cards_height))
call action(ac)
#
sd all_cards_w=cards_width
mult all_cards_w card_w
sd free_w
set free_w env_w
sub free_w all_cards_w
div free_w (cards_width+1)
#
sd all_cards_h=cards_height
mult all_cards_h card_h
sd free_h
set free_h env_h
sub free_h all_cards_h
div free_h (cards_height+1)
#
call sprintf(ac,"var free_w=%u;var free_h=%u;var total_pairs=%u
",free_w,free_h,(pairs))
call action(ac)

call action("
var card_increment=new Array();
var prev_card;var pairs;var moves;

function game_loaded()
{
    var i=0;var a;var inst_name;
    var pos=0;
    var row;var col;var back;
    while(i<cards.length)
    {
        a=0;
        while(a<4){
            card_at_pos(cards[i],pos,false);
            pos=pos+1;
            a=a+1;
        }
        i=i+1;
    }
    _text.message='Click to play';
    onMouseDown=game_at_start;
}
function start_no_function(){}
function game_at_start()
{
    var i=0;var a;var inst_name;var pos=0;
    while(i<cards.length)
    {
        a=0;
        while(a<4){
            inst_name=card_name(pos);
            this[inst_name].removeMovieClip();
            pos=pos+1;
            a=a+1;
        }
        i=i+1;
    }
    onMouseDown=start_no_function;
    newgame();
}
function newgame()
{
    _text.message='Ready';
    prev_card=0;
    pairs=total_pairs;
    moves=0;
    var row=0;var col;
    card_increment.length=0;var nr=0;
    while(row<cards_height)
    {
        col=0;
        while(col<cards_width)
        {
            card_increment.push(nr);nr=nr+1;
            col=col+1;
        }
        row=row+1;
    }

    var i=0;var a;
    while(i<cards.length)
    {
        a=0;
        while(a<4){
            var sorted_pos=random(card_increment.length);
            var pos=card_increment[sorted_pos];
            card_increment.splice(sorted_pos,1);

            card_at_pos(cards[i],pos,true);

            a=a+1;
        }
        i=i+1;
    }
}
function card_at_pos(name,pos,add_back)
{
    var inst_name=card_name(pos);
    attachMovie(name,inst_name,getNextHighestDepth());

    var row;var col;var back;
    row=pos/cards_width;row=int(row);back=row*cards_width;
    col=pos-back;

    var x=card_w*col;var free_on_col=1+col*free_w;x=x+free_on_col;
    var y=card_h*row;var free_on_row=1+row*free_h;y=y+free_on_row;

    var card_inst=this[inst_name];
    card_inst._x=x;card_inst._y=y;

    if(add_back==true)
    {
        var back_name='b_'+row+'_'+col;
        attachMovie('back',back_name,getNextHighestDepth());
        var back_inst=this[back_name];
        back_inst._x=x;back_inst._y=y;
    }
}
//name
function card_name(pos)
{
    var row;var col;var back;
    row=pos/cards_width;row=int(row);back=row*cards_width;
    col=pos-back;
    var inst_name='c_'+row+'_'+col;
    return inst_name;
}
game_loaded()
")

call submit_form(deffont,env_w,height,game_name)

call swf_exports_done()
call swf_showframe()
call swf_done()
return 1
