import java.util.*;

public class Place
{
    private String  name;
    private int     floor, x, y;

    private int     posInArray; // Used in the test case generator
    private Place   predecessor; // Used in the test case generator 
    private double  pathDistance; // Used in the test case generator 

    private boolean processed,
                    inQueue;

    private LinkedList<Place>   neighbours;

    public Place( String type, int floor, int x, int y )
    {
        name = String.format( "%s%02d%04d%04d", type, floor, x, y );

        this.floor = floor;
        this.x = x;
        this.y = y;

        this.processed=false;
        this.inQueue=false;

        this.neighbours = new LinkedList<Place>();
    }
    public Place( Scanner sf )
    {
        this.floor = sf.nextInt();
        this.x     = sf.nextInt();
        this.y     = sf.nextInt();
//        this.name  = sf.next();
        this.name  = String.format( "%03d%05d%05d", floor, x, y );

        this.processed=false;
        this.inQueue=false;

        this.neighbours = new LinkedList<Place>();
    }

    public String getName() { return name; }
    public int    getFloor() { return floor; }
    public int    getX() { return x; }
    public int    getY() { return y; }

    public int getPosInArray() { return posInArray; }
    public void setPosInArray( int pos ) { posInArray = pos; }
    public Place getPredecessor() { return predecessor; }
    public void setPredecessor( Place place ) { predecessor = place; }
    public double getPathDistance() { return pathDistance; }
    public void setPathDistance( double d ) { pathDistance = d; }

    public boolean getProcessed() { return processed; }
    public void    setProcessed( boolean p ) { processed=p; }
    public boolean getInQueue() { return inQueue; }
    public void    setInQueue( boolean q ) { inQueue=q; }

    public double distance( Place other )
    {
        int df = (this.floor - other.floor)*5;
        int dx = this.x - other.x;
        int dy = this.y - other.y;

        return Math.sqrt( df*df + dx*dx + dy*dy );
    }

    public void addNeighbour( Place other )
    {
        if ( other == this ) return;

        boolean exists=false;
        for( Place neighbour : neighbours ) {
            if ( neighbour == other ) {
                exists=true;
                break;
            }
        }

        if ( !exists ) {
            this.neighbours.add( other );
            other.neighbours.add( this );
        }
    }
    public int getCounterOfNeighbours()
    {
        return neighbours.size();
    }
    public LinkedList<Place> getNeighbours()
    {
        return neighbours;
    }

    public String toString()
    {
        //return String.format( "%d %d %d %s", floor, x, y, name );
        return String.format( "%d %d %d", floor, x, y );
    }
}
