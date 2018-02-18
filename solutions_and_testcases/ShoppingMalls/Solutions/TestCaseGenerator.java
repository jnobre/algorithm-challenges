import java.util.*;
import java.io.*;


public class TestCaseGenerator
{
    private static final double INFINITY = 9.9999e+300;

    final static String placeTypes[] = { "IP", "BAR", "RES", "SHOP" };

    final static int movements[][] = { { -1, -1 },
                                       { -1,  0 },
                                       { -1,  1 },
                                       {  0, -1 },
                                       {  0,  1 },
                                       {  1, -1 },
                                       {  1,  0 },
                                       {  1,  1 } };
    
    private Place map[][][];
    private boolean busy[][];

    private Random r;
    private Random s;
    
    private LinkedList<Place>       listOfPlaces;
    private LinkedList<Connection>  listOfConnections;

    private int maxX; // Meters in one side (width)
    private int maxY; // Meters in the other side (length)
    private int floors; // Number of floors (height)

    
    public TestCaseGenerator()
    {
        r = new Random();
        s = new Random();

        listOfPlaces = new LinkedList<Place>();
        listOfConnections = new LinkedList<Connection>();

/*
        maxX = 100 + r.nextInt( 101 );
        maxY = 100 + r.nextInt( 101 );
        floors = 1 + r.nextInt( 11 );
*
        maxX = 90 + r.nextInt( 21 );
        maxY = 90 + r.nextInt( 21 );
        floors = 5 + r.nextInt( 6 );
*/
        maxX = 40 + r.nextInt( 11 );
        maxY = 40 + r.nextInt( 11 );
        floors = 5 + r.nextInt( 3 );

        map = new Place [floors][maxX][maxY];
        busy = new boolean [maxX][maxY];
    }

    class Coordinates
    {
        public int f, x, y;

        public Coordinates( int f, int x, int y )
        {
            this.f = f;
            this.x = x;
            this.y = y;
        }
    }

    private Coordinates lookForNearestFreePosition( Coordinates pos )
    {
        if ( ! correctPosition( pos ) ) return null;
        if ( busy[ pos.x ][ pos.y ] == false && map[ pos.f ][ pos.x ][ pos.y ] == null ) return pos;


        Queue<Coordinates> pendingToVisit = new LinkedList<Coordinates>();
        boolean inQueue[][] = new boolean [ maxX ][ maxY ];

        pendingToVisit.add( pos );
        inQueue[pos.x][pos.y] = true;

        while( pendingToVisit.size() > 0 ) {

            pos = pendingToVisit.remove();

            for( int offset[] : movements ) {
            
                Coordinates newPos = new Coordinates( pos.f, pos.x+offset[0], pos.y+offset[1] );

                if ( correctPosition( newPos ) ) {

                    if ( busy[ newPos.x ][ newPos.y ] == false && map[ newPos.f ][ newPos.x ][ newPos.y ] == null ) return newPos;

                    if ( ! inQueue[newPos.x][newPos.y] ) {
                        pendingToVisit.add( newPos );
                        inQueue[newPos.x][newPos.y] = true;
                    }
                }
            }
        }
        return null;
    }
    private boolean correctPosition( int x, int y )
    {
        return ( 0 <= x && x < maxX  &&  0 <= y && y < maxY );
    }
    private boolean correctPosition( Coordinates pos )
    {
        return ( 0 <= pos.f && pos.f < floors  &&  0 <= pos.x && pos.x < maxX  &&  0 <= pos.y && pos.y < maxY );
    }

    public void putRandomPlaces( int perThousand )
    {
        // Put information points and connect them with stairs
        int x_for_stairs[] = { 5, maxX-5 };
        int y_for_stairs[] = { 5, maxY-5 };
        for( int x : x_for_stairs ) { 
            for( int y : y_for_stairs ) { 
                for( int f=0; f < floors; f++ ) {
                    map[f][x][y] = new Place( "IP", f, x, y );    busy[x][y] = true;
                    for( int g=0; g < f; g++ ) {
                        //map[f][x][y].addNeighbour( map[g][x][y] ); -- Don't add neighbours for allowing connections in the same floor
                        listOfConnections.add( new Connection( map[f][x][y], map[g][x][y], "stairs" ) );
                    }
                }
            }
        }
        System.err.println( "Added information points connected with stairs." );

        // Put information points and connect them with lifts
        int xy_for_lifts[][] = { { 5, maxY/2 }, { maxX/2, 5}, { maxX/2, maxY-5 }, {maxX-5, maxY/2} };

        for( int xy[] : xy_for_lifts ) {
            int x = xy[0], y = xy[1];

            for( int f=0; f < floors; f++ ) {
                map[f][x][y] = new Place( "IP", f, x, y );    busy[x][y] = true;
                for( int g=0; g < f; g++ ) {
                    //map[f][x][y].addNeighbour( map[g][x][y] ); -- Don't add neighbours for allowing connections in the same floor
                    listOfConnections.add( new Connection( map[f][x][y], map[g][x][y], "lift" ) );
                }
            }
        }
        System.err.println( "Added information points connected with lifts." );

        // Put information points and connect them with automatic stairs
        int offset[] = { -1, 1 };
        int x_for_automatic_stairs[] = { 8, maxX-8 };
        int y_for_automatic_stairs[] = { 8, maxY-8 };
        for( int x : x_for_automatic_stairs ) { 
            for( int y : y_for_automatic_stairs ) { 
                for( int f=0; f < floors; f++ ) {

                    map[f][x+offset[f%2]][y+offset[f%2]] = new Place( "IP", f, x+offset[f%2], y+offset[f%2] );
                      busy[x+offset[f%2]][y+offset[f%2]] = true;

                    if ( f > 0 ) {
                        int g=f-1;
                        if ( x == y ) {
                            // to go down
                            //map[f][x+offset[f%2]][y+offset[f%2]].addNeighbour( map[g][x+offset[g%2]][y+offset[g%2]] ); -- Don't add neighbours for allowing connections in the same floor
                            listOfConnections.add( new Connection( map[f][x+offset[f%2]][y+offset[f%2]], map[g][x+offset[g%2]][y+offset[g%2]], "escalator" ) );
                        } else {
                            // to go up
                            //map[g][x+offset[g%2]][y+offset[g%2]].addNeighbour( map[f][x+offset[f%2]][y+offset[f%2]] ); -- Don't add neighbours for allowing connections in the same floor
                            listOfConnections.add( new Connection( map[g][x+offset[g%2]][y+offset[g%2]], map[f][x+offset[f%2]][y+offset[f%2]], "escalator" ) );
                        }
                    }
                }
            }
        }
        System.err.println( "Added information points connected with automatic stairs." );

        // Generate random empty interior spaces
        int interiorSpaces = (int)(Math.sqrt(maxX*maxY)/10);
        while( --interiorSpaces >= 0 ) {
            int x0, x1, y0, y1;
            do {
                x0 = 10 + r.nextInt( maxX-20 );
                x1 = 10 + r.nextInt( maxX-20 );
            } while( x0+10 < x1 );
            do {
                y0 = 10 + r.nextInt( maxY-20 );
                y1 = 10 + r.nextInt( maxY-20 );
            } while( y0+10 < y1 );

            for( int x=x0; x <= x1; x++ ) {
                for( int y=y0; y <= y1; y++ ) {
                    busy[x][y] = true;
                }
            }
        }
        System.err.println( "Generated empty interior spaces." );

        for( int f=0; f < floors; f++ ) {
            for( int x=0; x < maxX; x++ ) {
                for( int y=0; y < maxY; y++ ) {
                    
                    if ( busy[x][y] || map[f][x][y] != null ) continue;

                    int width = 3 + r.nextInt( 13 );
                    int area  = 100 + r.nextInt( 101 );
                    int length = area/width;

                     width = Math.min( width,  maxX-x-1 );
                    length = Math.min( length, maxY-y-1 );

                    Coordinates pos = lookForNearestFreePosition( new Coordinates( f, x+width/2, y+length/2 ) );

                    if ( pos == null ) throw new RuntimeException( "Impossible to find a free position for a new place!" );

                    map[f][pos.x][pos.y] = new Place( placeTypes[s.nextInt(placeTypes.length)], f, pos.x, pos.y );

                    for( int w=x; w <= x+width; w++ ) {
                        for( int z=y; z <= y+length; z++ ) {
                            if ( !busy[w][z] && map[f][w][z] == null ) {
                                map[f][w][z] = map[f][pos.x][pos.y];
                            }
                        }
                    }
                }
            }
        }
        System.err.println( "Added random places." );
    }
    public void establishConnections()
    {
        for( int f=0; f < floors; f++ ) {

            Vector<Place> V = new Vector<Place>();

            Place place=null;
            for( int x=0; x < maxX; x++ ) {
                for( int y=0; y < maxY; y++ ) {

                    if ( !busy[x][y] && map[f][x][y] == null ) throw new RuntimeException( String.format( "hole in %d %d %d", f, x, y ) );

                    if ( map[f][x][y] != null  &&  map[f][x][y].getX() == x  &&  map[f][x][y].getY() == y ) {

                        listOfPlaces.add( map[f][x][y] ); // Add the definitive places to the list.
                        V.add( map[f][x][y] );

                        connectWithNeighbours( f, x, y );
                    }
                }
            }
            System.err.println( "Connected places in the same floor." );

            // Generating lists of connections in the same floor
            Queue<Place>  pendingPlaces = new LinkedList<Place>();

            place = V.get(0);
            pendingPlaces.add( place );
            place.setInQueue(true);

            while( pendingPlaces.size() > 0 ) {

                place = pendingPlaces.remove();
                place.setProcessed(true);

                for( Place neighgour : place.getNeighbours() ) {
                    if ( ! neighgour.getProcessed() ) {
                        listOfConnections.add( new Connection( place, neighgour ) );
                        if ( ! neighgour.getInQueue() ) {
                            pendingPlaces.add( neighgour );
                            neighgour.setInQueue(true);
                        }
                    }
                }
            }
            System.err.printf( "Generated the connections between places in the floor %d\n", f );
        }

        // Add as neighbours the places of different floors that are connected.
        for( Connection connection : listOfConnections ) {
            Place from = connection.getFrom();
            Place   to = connection.getTo();

            if ( from.getFloor() != to.getFloor() ) {
                from.addNeighbour( to );
                to.addNeighbour( from );
            }
        }
    }

    private void connectWithNeighbours( final int f, final int x, final int y )
    {
        Queue<Coordinates> pendingToVisit = new LinkedList<Coordinates>();

        boolean visited[][] = new boolean [ maxX ][ maxY ];
        boolean inQueue[][] = new boolean [ maxX ][ maxY ];

        visited[x][y] = true;

        for( int offset[] : movements ) {
            Coordinates pos = new Coordinates( f, x+offset[0], y+offset[1] );
            if ( correctPosition( pos ) ) {
                pendingToVisit.add( pos );
                inQueue[pos.x][pos.y] = true;
            }
        }

        while( pendingToVisit.size() > 0 ) {

            Coordinates pos = pendingToVisit.remove();
            inQueue[pos.x][pos.y] = false;
            visited[pos.x][pos.y] = true;

            if ( map[pos.f][pos.x][pos.y] == null ) continue;

            if ( map[pos.f][pos.x][pos.y] != map[f][x][y] ) {

                map[f][x][y].addNeighbour( map[pos.f][pos.x][pos.y] );

            } else {
                for( int offset[] : movements ) {

                    Coordinates newPos = new Coordinates( pos.f, pos.x+offset[0], pos.y+offset[1] );

                    if ( correctPosition( newPos ) && !visited[newPos.x][newPos.y] && !inQueue[newPos.x][newPos.y] ) {
                        pendingToVisit.add( newPos );
                        inQueue[newPos.x][newPos.y] = true;
                    }
                }
            }
        }
    }

    public void writeTo( PrintStream ps )
    {
        ps.printf( "%d %d\n", listOfPlaces.size(), listOfConnections.size() );

        // Generating the list of queries
        Place A[] = listOfPlaces.toArray( new Place [listOfPlaces.size()] );

        for( int i=0; i < A.length; i++ ) {
            A[i].setPosInArray(i);
            ps.println( A[i] );
        }

        for( Connection connection : listOfConnections ) ps.println( connection );

        // Preparing the matrix of distances
        double distances[][] = new double [A.length][A.length];
        for( int i=0; i < A.length; i++ ) {
            A[i].setPosInArray(i);

            distances[i][i] = 0.0;
            for( int j=i+1; j < A.length; j++ ) {
                distances[i][j] = distances[j][i] = INFINITY;
            }
        }

        for( Connection connection : listOfConnections ) {

            int from = connection.getFrom().getPosInArray();
            int   to = connection.getTo().getPosInArray();

            distances[from][to] = distances[to][from] = A[from].distance( A[to] );
            if ( connection.getType().equals( "escalator" ) ) {
                distances[from][to] = 1.0;
                distances[to][from] *= 3.0;
            } else if ( connection.getType().equals( "lift" ) ) {
                distances[from][to] = 1.0;
                distances[to][from] = 1.0;
            }
        }

        int queries[][] = new int [A.length*A.length][2];
        int nq=0;

        for( int i=0; i < A.length; i++ ) {
            for( int j=i+1; j < A.length; j++ ) {
                //if ( Math.random() < 0.5 && onlyOneShortestPath( A, distances, i, j ) )
                if ( onlyOneShortestPath( A, distances, i, j ) ) {
                    queries[nq][0] = i;
                    queries[nq][1] = j;
                    ++nq;
                }
            }
        }

        // ps.println();
        /*
        ps.printf( "%d\n", nq );
        for( int q=0; q < nq; q++ ) {
            // ps.printf( "%s %s\n", A[i].getName(), A[j].getName() );
            ps.printf( "%d %d\n", queries[q][0], queries[q][1] );
        }
        */
        int Q = 1000;
        ps.printf( "%d\n", Q );
        for( int i=0; i < Q; i++ ) {
            int q = r.nextInt( Q );
            ps.printf( "%d %d\n", queries[q][0], queries[q][1] );
        }
    }

    private double computeDistance( double distances[][], int path[] )
    {
        double distance = 0.0;
        for( int i=1; i < path.length; i++ ) distance += distances[ path[i-1] ][ path[i] ];

        return distance;
    }
    private boolean onlyOneShortestPath( Place places[], double distances[][], int from, int to )
    {
        int shortestPath[] = Dijkstra( places, distances, from, to );
        if ( shortestPath == null ) throw new RuntimeException( "Your Dijkstra implementation is wrong or the map has not all places connected!\n" );
        double shortestDistance = computeDistance( distances, shortestPath );

        for( int i=1; i < shortestPath.length; i++ ) {

            double distOfRemovedPath = distances[ shortestPath[i-1] ][ shortestPath[i] ];
                                       distances[ shortestPath[i-1] ][ shortestPath[i] ] = INFINITY;

            int path[] = Dijkstra( places, distances, from, to );

                                       distances[ shortestPath[i-1] ][ shortestPath[i] ] = distOfRemovedPath;

            if ( path != null ) {
                double dist = computeDistance( distances, path );

                if ( dist < shortestDistance ) throw new RuntimeException( "FATAL ERROR: review your code! dist=" + dist + "  shortestDistance=" + shortestDistance );
                if ( dist < shortestDistance+1 ) return false;
            }
        }

        return true;
    }

    public static int [] Dijkstra( Place places[], double distances[][], int from, int to )
    {
        for( Place place : places ) {
            place.setProcessed(false);
            place.setInQueue(false);
            place.setPathDistance(INFINITY);
        }

        Queue<Place> alivePlaces = new LinkedList<Place>();

        places[from].setPredecessor(null);
        places[from].setPathDistance(0);
        alivePlaces.add( places[from] );
        places[from].setInQueue(true);

        Place current = null;
        while( alivePlaces.size() > 0 ) {

            current = alivePlaces.remove();
            current.setInQueue(false);
            current.setProcessed(true);

            // if ( current == places[to] ) break; // This must be removed due to automatic stairs

            for( Place next : current.getNeighbours() ) {

                double dist = distances[ current.getPosInArray() ][ next.getPosInArray() ];
                if ( 0.0 < dist  &&  dist < INFINITY ) {

                    dist += current.getPathDistance();

                    if ( dist < next.getPathDistance()  &&  dist < places[to].getPathDistance() ) {

                        next.setPathDistance(dist);
                        next.setPredecessor(current);
                        next.setProcessed(false);
                        if ( ! next.getInQueue() ) {
                            alivePlaces.add( next );
                            next.setInQueue(true);
                        }
                    }
                }
            }
        }

        if ( places[to].getPathDistance() >= INFINITY || places[to].getPredecessor() == null ) {
            throw new RuntimeException( "Destination not reached!  distances["+places[from].getName()+"]["+places[to].getName()+"] = " + distances[from][to] );
        }

        Stack<Integer> s = new Stack<Integer>();
        Place p = places[to];
        while( p != null ) {
            s.add( p.getPosInArray() );
            p = p.getPredecessor();            
        }

        int path[] = new int [s.size()];
        for( int i=0; s.size() > 0; i++ ) path[i] = s.pop();

        if ( path[0] != from || path[path.length-1] != to )
            throw new RuntimeException( String.format( "path[0]=%d from=%d AND path[%d]=%d to=%d", path[0], from, path.length-1, path[path.length-1], to ) );

        return path;
    }

    public static void main( String args[] )
        throws Exception
    {

        TestCaseGenerator tcg = new TestCaseGenerator();

        tcg.putRandomPlaces( 10 );
        tcg.establishConnections();

        tcg.writeTo( System.out );
    }
}
