str leaps_format="
var distance;
var angle;
var ballX;
var ballY;
var advanceX=7;
var difX;var difY;
var a;var c;
//hit angle, vine [0,60]
function start_fly(hitpos)
{
    var minangle=30;
    ballX=_ball._x;

    //skill angle
    //var maxangle=90-minangle;
    //var difangle=maxangle-minangle;
    //angle=difangle*hitpos/(aici era de la actionf)+minangle+difangle;
	angle=hitpos/%u+minangle;

    //find max raising and the projected distance
    var ballT=%u+hitpos;
    //var maxascendentdistance=ballT*100000/sine[90-minangle];
    //e intre 30 si 60 ca daca ar fi x/0 nu ar fi bine
    var maxascendentdistance=ballT/Math.cos(minangle*Math.PI/180);
    //translate this distance to the current angle to find the first distance/2
    //var xlandfromball=maxascendentdistance*sine[angle]/100000;
    var xlandfromball=maxascendentdistance*Math.sin(angle*Math.PI/180);

    //first distance
    distance=xlandfromball*2;

    start_from_ground();
    return angle;
}
function start_from_ground()
{
    if(distance<30)
    {
        _env1.stop();
        submitform(score);
        return undefined;
    }
    score=score+distance;

    //x1 y1
    var x1=ballX;
    var y1=_ball._y;
    //x2 y2
    var width=distance/2;
    //var dist=width*100000/sine[angle];
    //e intre 30 si 60 ca daca ar fi x/0 nu ar fi bine
    var dist=width/Math.sin(angle*Math.PI/180);
    //var height=dist*sine[90-angle]/100000;
    var height=dist*Math.cos(angle*Math.PI/180);
    var x2=x1-width;
    var y2=y1-height;
    //x3 y3
    var x3=x1-distance;
    var y3=y1;

    //parabole to radix x=0 and vertex y=0
    var nx1=x1-x2;var ny1=0;
    var nx2=0;var ny2=y2-y1;
    var nx3=x3-x2;var ny3=0;

    //difX difY for back to game coord
    difX=nx1-x1;difY=ny1-y1;

    //parabole a b=0 c
    c=ny2;
    var alpha=ny1+ny3;
    var q=2*c;
    alpha=alpha-q;
    var beta=nx1*nx1;
    q=nx3*nx3;
    beta=beta+q;
    a=alpha/beta;
}
function move()
{
    ballX=ballX-advanceX;
    var xball=ballX+difX;
    var alpha=xball*xball*a;
    var yball=alpha+c;
    ballY=yball-difY;

    if(_ball._y<ballY)
    {
        if(ballY>%u)
        {
            var maxdecrement=distance/2;
            var value=random(maxdecrement);
			//nu e bine la IT sa nu se mai termine daca random vine numai 0
            distance=distance-value-1;
            start_from_ground();
            return undefined;
        }
    }

    _ball._y=ballY;
    _bat._x=_bat._x+advanceX;
    var newx1=_env1._x+advanceX;
    var newx2=_env2._x+advanceX;
    var wd=%u;
    if(newx1>wd)
    {
        newx1=newx2-wd;
    }
    if(newx2>wd)
    {
        newx2=newx1-wd;
    }
    _env1._x=newx1;
    _env2._x=newx2;

    //_env1.gotoandplay(2);
}
"
sd land_hit
set land_hit bat_y
sub land_hit ball_height
#call sprintf(ac,leaps_format,ball_height,bat_y,land_hit,envwidth)
call sprintf(ac,leaps_format,(step),bat_y,land_hit,envwidth)
call action(ac)
