float err[3],olderr[3],f[3],fp[3],fd[3],fi[3],gd,gp;
float item[3],vel,dist,mypos[3],att[3];
float area[4],oldstato,p1[3],p2[3],centro[3],distmin,diff;
int upperlimit, lowerlimit,stato,i,c,t;
bool b,d;
ZRState mystate;




void init()
{
    upperlimit=1;
    lowerlimit=-1;
    stato=0;
    gp=0.5;
    api.setPosGains( 0.5, 0 ,6);
    t=0;
    for(i=0;i<3;i++)
    {
        p1[i]=0;
        p2[i]=0;
        centro[i]=0;
    }
    
}






void loop()
{
    if(t==0) d=bor();
    api.getMyZRState (mystate);
    game.getZone(area);
    b=game.dockItem();
    t++;
    if(b==1) stato=2;
    
    
    
    
    switch(stato)
    {
        
        case 0:    
                game.dropSPS();
                game.dropSPS();
                game.dropSPS();
                stato=1;
        break;
        
        case 1:
                gd=8;
                distmin=100;
                c=0;
                for(i=0;i<5;i++)
                {
                
                    game.getItemLoc(item, i);
            
                    mathVecSubtract (err, item, centro, 3);
                    dist=mathVecMagnitude(err,3);
            
                    if(dist>0.1)
                    {
                        mathVecSubtract (err, item, mystate, 3);
                        dist=mathVecMagnitude(err,3);
            
        
                        if(dist<distmin && game.hasItem(i)==0) 
                        {
                            distmin=dist;
                            c=i;
                        }
                    }
                }
            
            
                game.getItemLoc(item, c);
            
                mathVecSubtract (att, item, mystate, 3);
            
                dist=mathVecMagnitude(att,3);
            
                mathVecNormalize (att, 3);
                api.setAttitudeTarget(att);
            
                if(dist>0.15) pospid(item);
                
        break;
        
        case 2:
                gd=10;
                mathVecSubtract (att, centro, mystate, 2);
                mathVecNormalize (att, 2);
                api.setAttitudeTarget(att);
        
                mathVecSubtract (err, centro, mystate, 3);
                dist=mathVecMagnitude(err,3);
        
                mathVecNormalize (err, 3);
                mathVecSubtract(att,err,&mystate[6],3);
                diff=mathVecMagnitude(att,3);
        
                if(dist<0.2)
                {
                    if(diff<0.3)
                    {
                        game.dropItem();
                        stato=1;
                    }
                }
                else  if(dist>0.2) pospid(centro);//api.setPositionTarget(area);
        
                DEBUG(("dist:%f    diff:%f",dist,diff));
        
        break;
        
    }
    
    
    
    
    
    
    
    
    
    
    if(t>2)
    for(i=0;i<3;i++)
                {
                        if(area[i]>p1[i]) p1[i]=area[i]; 
                        else if(area[i]<p2[i]) p2[i]=area[i]; 
                        //DEBUG(("AREA%d:%f",i,area[i]));
                        centro[i]=(p1[i]+p2[i])/2;
                }
    
    
    
}









void pospid(float it[3])
{
    mathVecSubtract (err, it, mystate, 3);
    dist=mathVecMagnitude(err,3);
    vel=mathVecMagnitude(&mystate[3],3);
    
    for(i=0;i<3;i++)
        {
            api.getMyZRState (mystate);
            mathVecSubtract (err, it, mystate, 3);//calcolo errore
            fp[i]=gp*err[i];//calcolo forza proporzionale
            if(fp[i]>upperlimit) fp[i]=upperlimit;
            if(fp[i]<lowerlimit) fp[i]=lowerlimit;
            
             if(t>0) 
            {
                fd[i]=gd*(err[i]-olderr[i]);//calcolo forza derivatrice  5.8 
                if(fd[i]>upperlimit+1) fd[i]=upperlimit+1;//+1 e -1 perche deve riuscire a contrastare e invertire la forza proporzionale se necessario
                if(fd[i]<lowerlimit-1) fd[i]=lowerlimit-1;
            }
            else fd[i]=0;
                    
                     
            f[i]=fp[i]+fd[i];//somma delle forze calcolate
            olderr[i]=err[i];
        }
        c=0;
        for(i=0;i<2;i++)
        if(fabs(f[i])<0.055 && stato<3) 
        {
            DEBUG(("INERZIA :%d",i));
            f[i]=0;
            c++;
        }
    f[2]=0;
    
    if(c!=2 || stato>2) api.setForces(f);
}








bool bor()
{
    bool colore;
    if(mystate[1]>0) colore = true;
    else colore = false;
    
    return colore;
}
