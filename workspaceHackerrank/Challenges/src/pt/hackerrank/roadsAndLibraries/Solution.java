package pt.hackerrank.roadsAndLibraries;

import java.util.*;

/**
*https://www.hackerrank.com/challenges/torque-and-development/problem solution
**/
public class Solution {

    static long cost(long cLib, long cRoad, ArrayList<List<Integer>> g, int gSize)  {
        if (cLib <= cRoad)  {
            return cLib * (long)gSize;
        }
        int discon = numDisconnected(g);
        return (cRoad * (gSize - discon)) + (cLib * discon);
    }

    static int numDisconnected(ArrayList<List<Integer>> adj)  {
        int result = 0;
        HashSet<Integer> explored = new HashSet<>();
        int length = adj.size();
        for (int i = 0; i < length; i++) {
            if (!explored.contains(i)) {
                Stack<Integer> stack = new Stack<>();
                stack.push(i);
                while (!stack.empty()) {
                    int curr = stack.pop();
                    explored.add(curr);
                    for (int neighbor : adj.get(curr)) {
                        if (!explored.contains(neighbor)) {
                            stack.push(neighbor);
                        }
                    }
                }

                result += 1;
            }
        }
        return result;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int q = in.nextInt();
        for(int a0 = 0; a0 < q; a0++){
            int nCities = in.nextInt();
            ArrayList<List<Integer>> adj = new ArrayList<List<Integer>>(nCities);
            for (int i = 0; i < nCities; i++) {
                adj.add(new ArrayList<Integer>());
            }
            int nRoads = in.nextInt();
            long cLib = in.nextLong();
            long cRoad = in.nextLong();
            for (int i = 0; i < nRoads; i++) {
                int city_1 = in.nextInt() - 1;
                int city_2 = in.nextInt() - 1;
                adj.get(city_1).add(city_2);
                adj.get(city_2).add(city_1);
            }
            System.out.println(cost(cLib, cRoad, adj, nCities));
        }
    }
}