import java.util.*;

public class Connection
{
    private static Random r = new Random();
    private static String types[] = { "walking", "stairs", "lift", "escalator" };

    private Place   from, to;
    private String  type;
    private double  distance;

    public Connection( Place from, Place to )
    {
        this.from = from;
        this.to   = to;
        if ( from.getFloor() == to.getFloor() ) {
            this.type = "walking";
        } else if ( from.getX() == to.getX()  &&  from.getY() == to.getY() ) {
            this.type = types[r.nextInt(2)];
        } else {
            this.type = types[2];
        }

        this.distance = from.distance( to );
    }
    public Connection( Place from, Place to, String type )
    {
        this.from = from;
        this.to   = to;
        this.type = type;
        this.distance = from.distance( to );
    }

    public double getDistance() { return distance; }
    public Place  getFrom() { return from; }
    public Place  getTo() { return to; }
    public String getType() { return type; }

    public String toString()
    {
        //return String.format( "%s %s %s", from.getName(), to.getName(), type );
        return String.format( "%d %d %s", from.getPosInArray(), to.getPosInArray(), type );
    }
}
