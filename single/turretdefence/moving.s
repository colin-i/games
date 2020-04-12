

#machines are moving

call add_enemy("D:/universe/local/turretdefence/enemies/tank.dbl","tank_i",tk)
call add_enemy("D:/universe/local/turretdefence/enemies/plane.dbl","plane_i",pl)
#
setcall presprite swf_sprite_new()
setcall img swf_image("D:/universe/local/turretdefence/shot.dbl")
call swf_sprite_placeobject(presprite,img,0)
call swf_sprite_showframe(presprite)
setcall sprite swf_sprite_done(presprite)
call swf_exports_add(sprite,"fire")
#

call action("
var field_enemies;var free_enemies;var destroyed_enemies;
var set_en_nr=new Array();var set_en_name=new Array();var set_en_parent=new Array();
function start_round()
{
    field_enemies=round_data[current_round][1];free_enemies=0;farest_pos=-1;
    destroyed_enemies=0;

    var r=current_round+1;_text.message='Round '+r;
    var half_random=int(field_enemies/2);var half_half_random=int(half_random/2);
    var ground_nr=random(half_random)+1;ground_nr=ground_nr+half_half_random;
    var air_nr=field_enemies-ground_nr;
    set_en_nr.push(ground_nr);set_en_nr.push(air_nr);
    set_en_name.push('tank');set_en_name.push('plane');
    set_en_parent.push(road['enemies_ground']);set_en_parent.push(road['enemies_air']);
    var en_x=en_start_x;var en_y=0-cell;
    var mv;
    var i=0;while(i<field_enemies){
        var nr=random(set_en_nr.length);
        var name=set_en_name[nr];
        var container=set_en_parent[nr];
        set_en_nr[nr]--;
            //<=0 only for debugging when we have 1 enemy tests
        if(set_en_nr[nr]<=0){set_en_nr.splice(nr,1);set_en_name.splice(nr,1);set_en_parent.splice(nr,1);}
        container.attachMovie(name,name+i,container.getNextHighestDepth());
        mv=container[name+i];
        mv._x=en_x;mv._y=en_y;
        if(en_x==en_start_x)en_x=en_x+cell;else{en_x=en_start_x;en_y=en_y-cell;}
        i++;
    }
}
//bool false if destroyed
function move_enemy(enemy)
{
    if(enemy.hit_points<=0){
        destroyed_enemies++;
        field_enemies--;
        if(field_enemies==0)round_over();
        return false;
    }
    if(enemy.sens=='D'){
        enemy._y=enemy._y+cell;
        if(enemy._y<0)return undefined;
        var max_h=grid_lat*cell;
        if(enemy._y>=max_h)
        {
            if(enemy._y==max_h){
                field_enemies--;
                if(field_enemies==0)convoy_escaped();
                else if(enemy.escaped==1)free_enemies--;
            }
            return true;
        }
    }
    else if(enemy.sens=='L')enemy._x=enemy._x-cell;
    else enemy._x=enemy._x+cell;

    test_defense(enemy);

    var col=enemy._x/cell;var row=enemy._y/cell;var sens=senses[row][col];
    if(enemy.sens!=sens)
    {
        if(sens=='L'){enemy.mov._rotation=90;enemy.mov._x=enemy.mov._x+enemy.mov._width;}
        else if(sens=='R'){enemy.mov._rotation=-90;enemy.mov._y=enemy.mov._y+enemy.mov._height;}
        else{
            enemy.mov._x=0;enemy.mov._y=0;
            enemy.mov._rotation=0;
        }
        enemy.sens=sens;
    }
    return true;
}

function convoy_escaped(){
    //this function is called at convoy escaped or finish line passed by last enemy
    if(farest_pos!=maximum_pos){
        farest_pos=maximum_pos;round_over();
    }
}
function round_over(){
    score_calculation();
    var round_score=maximum_pos-farest_pos+1;
    score=score+round_score+destroyed_enemies;
    prev_turrets=prev_turrets+round_turrets;
    current_round++;
    if(current_round!=all_rounds)new_round();
    else{score_up();submitform(score);}
}
")

#turrets are shoting

call action("
var left_air=new Array();var left_ground=new Array();var right_air=new Array();var right_ground=new Array();
function test_defense(enemy){
    var line=enemy.line;var pos=enemy.pos;
                        //nobody will shot this enemy until the end
    if(pos==line.length)
    {
        if(enemy.escaped==0)
        {
            enemy.escaped=1;free_enemies++;
        }
        //call here, if there is 1 unescaped and is destroyed that frame,
            //this will be called by the escaped machines that are still moving
        if(free_enemies>=field_enemies)convoy_escaped();
        return undefined;
    }
    var pos_line=enemy.pos_line;enemy.pos_line++;
    //update for score
    if(pos_line>farest_pos)farest_pos=pos_line;

    var hit_at=line[pos][0];
                        //the shot will come in a following square
    if(pos_line<hit_at)return undefined;
    //test if there is a tower that can shot this frame
    var towers=line[pos][1];
    var i=0;while(i<towers.length){
        var tower=towers[i];
        if(tower.waiting==true){
            tower.waiting=false;
            tower.play();
            enemy.fire_mc._visible=true;
            var en_type=enemy.code_name;
            var min_dmg=tower[en_type+'_min'];var max_dmg=tower[en_type+'_max'];
            var damage=random(max_dmg-min_dmg)+min_dmg;
            enemy.hit_points=enemy.hit_points-damage;
            if(enemy.hit_points<0)enemy.hit_points=0;
            var broken_point=enemy.hit_points*cell/enemy.all_hits;
            enemy.broken.lineTo(broken_point,0);
        }
        i++;
    }
    //update to a new hit line pos
    enemy.pos++;
}
")
