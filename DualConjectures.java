package io.github.MathIsFun0.DualConjectures;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.*;

public class DualConjectures {
    static Scanner scanner = new Scanner(System.in);
    static Conjecture conjecture = new Conjecture();
    static ArrayList<int[]> trivialFactors = new ArrayList<>();
    static Options options;

    public static void main(String[] args) throws InterruptedException, IOException {
        options = new Options(String.join("\n", Files.readAllLines(Path.of(System.getProperty("user.dir") + "\\DualRieselClient.ini"))));
        System.out.println("What do you want to run today?\n\n[0] Continue from a save.\n[1] Run a test from scratch.");
        String response = scanner.next();
        switch (response) {
            case "0" -> readFile();
            case "1" -> initialization();
            default -> System.out.println("Invalid response, quitting...");
        }
    }

    static void readFile() throws IOException {
        System.out.println("Enter the file path.");
        String filePath = scanner.next();
        String fileContent = String.join("\n", Files.readAllLines(Path.of(filePath)));
        conjecture = new Conjecture(fileContent);
        conjecture.fileName = filePath;
        switch (conjecture.state) {
            case "Initial Testing" -> initialTesting();
            case "Sieving" -> sieveAndTest();
            case "Testing" -> sieveAndTest();
            default -> initialTesting();
        }
    }

    static void initialization() throws IOException {
        System.out.println("What type of dual conjecture are you testing? Enter S for dual Sierpinski conjectures, Ra for dual Riesel conjectures that include negative terms. or Rp for dual Riesel conjectures that exclude negative terms.");
        conjecture.conjectureType = scanner.next();
        if (!Arrays.asList(new String[]{"S", "Ra", "Rp"}).contains(conjecture.conjectureType)) {
            System.out.println("Invalid response, please try again.");
            initialization();
            return;
        }
        System.out.println("Please enter the base of your conjecture. (The b in b^n+/-k).");
        conjecture.base = scanner.nextInt();
        if (conjecture.base < 2) {
            System.out.println("Invalid response, please try again.");
            initialization();
            return;
        }
        System.out.println("Please enter the minimum candidate that you would like to test. (The k in b^n+/-k).");
        conjecture.minK = scanner.nextLong();
        if (conjecture.minK < 0) {
            System.out.println("Invalid response, please try again.");
            initialization();
            return;
        }
        System.out.println("Please enter the maximum candidate that you would like to test. (The k in b^n+/-k). If you are unsure, you can use the conjectured k minus 1.");
        conjecture.maxK = scanner.nextLong();
        if (conjecture.maxK < conjecture.minK) {
            System.out.println("Invalid response, please try again.");
            initialization();
            return;
        }
        System.out.println("Please enter the minimum exponent that you would like to test. (The n in b^n+/-k). Use 1 if you want to start from scratch.");
        conjecture.minN = scanner.nextInt();
        if (conjecture.minN < 1) {
            System.out.println("Invalid response, please try again.");
            initialization();
            return;
        }
        System.out.println("Please enter the maximum exponent that you would like to test. (The n in b^n+/-k). Use 0 if you do not have a maximum.");
        conjecture.maxN = scanner.nextInt();
        if (conjecture.maxN < conjecture.minN && conjecture.maxN != 0) {
            System.out.println("Invalid response, please try again.");
            initialization();
            return;
        }
        if (conjecture.maxN == 0) {
            conjecture.maxN = Integer.MAX_VALUE;
        }
        if (conjecture.maxN != Integer.MAX_VALUE) {
            System.out.printf("Is the following information correct?\nConjecture: %1s%2$1d\nCandidates: %3$1d-%4$1d\nExponents: %5$1d-%6$1d\n\nPlease enter Y/N.%n", conjecture.conjectureType.charAt(0), conjecture.base, conjecture.minK, conjecture.maxK, conjecture.minN, conjecture.maxN);
        } else {
            System.out.printf("Is the following information correct?\nConjecture: %1s%2$1d\nCandidates: %3$1d-%4$1d\nExponents: Starting at %5$1d.\n\nPlease enter Y/N.%n", conjecture.conjectureType.charAt(0), conjecture.base, conjecture.minK, conjecture.maxK, conjecture.minN);
        }
        String decision = scanner.next();
        switch (decision) {
            case "Y", "y" -> getTrivialFactors();
            default -> initialization();
        }
    }

    static void getTrivialFactors() throws IOException {
        //type 0: trivial factors
        ArrayList<Integer> factorsOfN = primeFactors(conjecture.base);
        ArrayList<Integer> factorsOfN_1 = primeFactors(conjecture.base - 1);
        for (int i : factorsOfN) {
            trivialFactors.add(new int[]{0, 0, i});
        }
        if (conjecture.conjectureType.equals("S")) {
            for (int i : factorsOfN_1) {
                trivialFactors.add(new int[]{0, i - 1, i});
            }
        } else {
            for (int i : factorsOfN_1) {
                trivialFactors.add(new int[]{0, 1, i});
            }
        }
        //type 1: perfect powers
        if (isPerfectPower(conjecture.base) > 1) {
            ArrayList<Integer> powList = allPerfectPowers(conjecture.base);
            for (int i : powList) {
                trivialFactors.add(new int[]{1, i});
            }
        }
        //type 2: special squares
        if (!conjecture.conjectureType.equals("S")) {
            ArrayList<Integer> factorsOfNPlus1 = primeFactors(conjecture.base + 1);
            for (int i : factorsOfNPlus1) {
                if (i % 4 == 1) {
                    for (int j = 2; j < i; j++) {
                        if ((j * j) % i == i - 1) {
                            trivialFactors.add(new int[]{2, j, i});
                            break;
                        }
                    }
                }
            }
        }
        System.out.println("Trivial factors and algebraic sets auto-calculated. Is this correct?");
        for (int[] factor : trivialFactors) {
            switch (factor[0]) {
                case 0 -> System.out.printf("k = %1$1d mod %2$1d%n", factor[1], factor[2]);
                case 1 -> System.out.printf("k = m^%1$1d%n", factor[1]);
                case 2 -> System.out.printf("k = m^2, m = %1$1d, %2$1d mod %3$1d%n", factor[1], factor[2] - factor[1], factor[2]);
                case 3 -> System.out.printf("k = %1$1dm^2, m = %2$1d, %3$1d mod %4$1d%n", factor[1], factor[2], factor[3] - factor[2], factor[3]);
                case 4 -> System.out.printf("k = %1$1d%n", factor[1]);
            }
        }
        System.out.println("Please enter Y to continue, N to add factors, or C to clear all factors and add factors.");
        String decision = scanner.next();
        switch (decision) {
            case "Y", "y" -> parseFilters();
            case "C", "c" -> {
                trivialFactors = new ArrayList<>();
                askForFactors();
            }
            default -> askForFactors();
        }
    }

    static void askForFactors() throws IOException {
        System.out.println("Enter a factor, using the format on https://docs.google.com/document/d/1fkCgsESiV2CuZ48Vh3TTh35bOEq21gsc657teOQViWI/edit.");
        String output = scanner.next();
        String[] arrayOutput = output.split(",");
        int[] intOutput = new int[arrayOutput.length];
        for (int i = 0; i < intOutput.length; i++) {
            intOutput[i] = Integer.parseInt(arrayOutput[i]);
        }
        trivialFactors.add(intOutput);
        System.out.println("Is this correct?");
        for (int[] factor : trivialFactors) {
            switch (factor[0]) {
                case 0 -> System.out.printf("k = %1$1d mod %2$1d%n", factor[1], factor[2]);
                case 1 -> System.out.printf("k = m^%1$1d%n", factor[1]);
                case 2 -> System.out.printf("k = m^2, m = %1$1d, %2$1d mod %3$1d%n", factor[1], factor[2] - factor[1], factor[2]);
                case 3 -> System.out.printf("k = %1$1dm^2, m = %2$1d, %3$1d mod %4$1d%n", factor[1], factor[2], factor[3] - factor[2], factor[3]);
                case 4 -> System.out.printf("k = %1$1d%n", factor[1]);
            }
        }
        System.out.println("Please enter Y to continue, N to add factors, or C to clear all factors and add factors.");
        String decision = scanner.next();
        switch (decision) {
            case "Y", "y" -> parseFilters();
            case "C", "c" -> {
                trivialFactors = new ArrayList<>();
                askForFactors();
            }
            default -> askForFactors();
        }
    }

    static ArrayList<Integer> primeFactors(int n) {
        ArrayList<Integer> factors = new ArrayList<>();
        if (n % 2 == 0) {
            factors.add(2);
            while (n % 2 == 0) {
                n /= 2;
            }
        }
        for (int i = 3; i <= Math.sqrt(n); i += 2) {
            if (n % i == 0) {
                factors.add(i);
                while (n % i == 0) {
                    n /= i;
                }
            }
        }
        if (n != 1) {
            factors.add(n);
        }
        return factors;
    }

    static int isPerfectPower(int n) {
        for (int i = 2; i <= Math.sqrt(n); i++) {
            int j = i * i;
            int p = 2;
            while (j <= n) {
                if (j != n) {
                    j *= i;
                    p++;
                } else {
                    return p;
                }
            }
        }
        return 0;
    }

    static ArrayList<Integer> allPerfectPowers(int n) {
        ArrayList<Integer> q = new ArrayList<>();
        for (int i = 2; i <= Math.sqrt(n); i++) {
            int j = i * i;
            int p = 2;
            while (j <= n) {
                if (j != n) {
                    j *= i;
                    p++;
                } else {
                    q.add(p);
                    break;
                }
            }
        }
        return q;
    }

    static void parseFilters() throws IOException {
        for (long i = 1; i <= conjecture.maxK; i++) {
            boolean filtered = false;
            for (int[] trivialFactor : trivialFactors) {
                switch (trivialFactor[0]) {
                    case 0 -> {
                        if (i % trivialFactor[2] == trivialFactor[1]) { //i = t[1] mod t[2]
                            filtered = true;
                        }
                    }
                    case 1 -> {
                        if (Math.pow(i, 1.0 / trivialFactor[1]) % 1.0 == 0.0) { //nth root of i is an integer
                            filtered = true;
                        }
                    }
                    case 2 -> {
                        if (Math.sqrt(i) % 1 == 0) {
                            if (Math.sqrt(i) % trivialFactor[2] == trivialFactor[1] || Math.sqrt(i) % trivialFactor[2] == trivialFactor[2] - trivialFactor[1]) {
                                filtered = true;
                            }
                        }
                    }
                    case 3 -> {
                        if (Math.sqrt((double) i / trivialFactor[1]) % 1 == 0) {
                            if (Math.sqrt((double) i / trivialFactor[1]) % trivialFactor[3] == trivialFactor[2] || Math.sqrt((double) i / trivialFactor[1]) % trivialFactor[3] == trivialFactor[3] - trivialFactor[2]) {
                                filtered = true;
                            }
                        }
                    }
                    case 4 -> {
                        if (i == trivialFactor[1]) {
                            filtered = true;
                            trivialFactors.remove(trivialFactor);
                        }
                    }
                }
            }
            if (!filtered) {
                conjecture.candidates.add(i);
            }
        }
        conjecture.fileName = conjecture.conjectureType + conjecture.base + "-" + conjecture.minK + "-" + conjecture.maxK + "-" + conjecture.minN + ".ini";
        conjecture.state = "Initial Testing";
        try {
            conjecture.saveToFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
        initialTesting();
    }

    static void initialTesting() throws IOException {
        int maxSize = (int) Math.floor(5000.0d / Math.log10(conjecture.base));
        if (maxSize > conjecture.maxN) {
            maxSize = conjecture.maxN;
        }
        String sign = "-";
        if (conjecture.conjectureType.equals("S")) {
            sign = "+";
        }
        int N = conjecture.minN;
        int maxThreads = options.pfgwInstances;
        while (conjecture.minN <= maxSize) {
            if (conjecture.startingN == conjecture.minN) {
                System.out.println("Testing small values - n < " + maxSize);
            }
            ArrayList<String> candidateString = new ArrayList<>();
            if (conjecture.candidates.size() > 10000) {
                System.out.println("Creating candidate list, this may take a while...");
            }
            if (conjecture.candidates.size() <= 128 * maxThreads) {
                for (int i = 0; i < (double) conjecture.candidates.size() / 128; i++) {
                    candidateString.add("");
                    for (int j = 128 * i; j < Math.min(128 * (i + 1), conjecture.candidates.size()); j++) {
                        long k = conjecture.candidates.get(j);
                        if (!(conjecture.conjectureType.equals("Rp")) || (Math.log(k) / Math.log(conjecture.base) < N)) {
                            candidateString.set(i, candidateString.get(i).concat(conjecture.base + "^" + N + sign + k + "\r\n"));
                        }
                    }
                }
            } else {
                int numPerThread = (int) Math.floor((double) conjecture.candidates.size() / maxThreads) + 1;
                for (int i = 0; i < (double) conjecture.candidates.size() / numPerThread; i++) {
                    candidateString.add("");
                    for (int j = numPerThread * i; j < Math.min(numPerThread * (i + 1), conjecture.candidates.size()); j++) {
                        long k = conjecture.candidates.get(j);
                        if (!(conjecture.conjectureType.equals("Rp")) || (Math.log(k) / Math.log(conjecture.base) < N)) {
                            candidateString.set(i, candidateString.get(i).concat(conjecture.base + "^" + N + sign + k + "\r\n"));
                        }
                    }
                }
            }
            System.out.println("Testing n = " + N + "...");
            String pathPrefix = "temp/" + conjecture.conjectureType + conjecture.base + "-" + conjecture.minK + "-" + conjecture.maxK + "-" + N + "_";
            for (int i = 0; i < candidateString.size(); i++) {
                try {
                    Files.createFile(Path.of(pathPrefix + i + ".txt"));
                } catch (IOException e) {
                }
                FileWriter fileWriter = new FileWriter(pathPrefix + i + ".txt", false);
                fileWriter.write(candidateString.get(i));
                fileWriter.close();
            }

            //Finally, we actually get to run other people's code!
            Files.deleteIfExists(Path.of("pfgw-prime.log"));
            Files.deleteIfExists(Path.of("pfgw.log"));
            String path = System.getProperty("user.dir") + "\\" + options.pfgwPath;
            Process[] pfgw = new Process[candidateString.size()];
            for (int i = 0; i < pfgw.length; i++) {
                pfgw[i] = new ProcessBuilder().command(path, "-C\"quiet\"", "-f", "-T"+options.pfgwThreadsPerInstance, pathPrefix + i + ".txt").inheritIO().start();
            }
            for (Process process : pfgw) {
                try {
                    process.waitFor();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            try {
                Files.createFile(Path.of("pfgw-prime.log"));
            } catch (Exception e) {
                //file already exists
            }
            try {
                Files.createFile(Path.of("pfgw.log"));
            } catch (Exception e) {
                //file already exists
            }
            List<String> results = Files.readAllLines(Path.of("pfgw-prime.log"));
            results.addAll(Files.readAllLines(Path.of("pfgw.log")));
            for (String s : results) {
                long result = Long.parseLong(s.split(sign)[1]);
                conjecture.candidates.remove(result);
            }
            FileWriter fileWriter = new FileWriter("DRC_results.txt", true);
            results.add("");
            fileWriter.write(String.join("\n",results));
            fileWriter.close();
            N++;
            conjecture.minN++;
            conjecture.saveToFile();
        }
        if (conjecture.minN > conjecture.maxN) {
           System.out.println("Limit reached.");
        } else if (conjecture.candidates.size() == 0) {
            System.out.println("All candidates have been eliminated!");
        }
        sieveAndTest();
    }

    static void sieveAndTest() throws IOException {
        if (!conjecture.state.equals("Testing")) {
            conjecture.state = "Sieving";
            conjecture.saveToFile();
        }
        while (conjecture.step < conjecture.minN) {
            conjecture.step += options.stepSize;
        }
        if (conjecture.state.equals("Sieving")) {
           System.out.println("Sieving "+conjecture.candidates.size()+" candidates from n = "+conjecture.minN+"-"+conjecture.step);
           long sieveLimit = (long)Math.floor(conjecture.step * Math.log(conjecture.base) * options.stepSize * options.sieveFactor);
           List<List<Long>> sieveCandidates = new LinkedList<>();
           int partitionSize = (int)Math.floor(conjecture.candidates.size()/options.sieveInstances);
           for (int i = 0; i < conjecture.candidates.size(); i += partitionSize) {
                sieveCandidates.add(conjecture.candidates.subList(i,
                        Math.min(i + partitionSize, conjecture.candidates.size())));
           }
           String sign = "-";
           if (conjecture.conjectureType.equals("S")) {
               sign = "";
           }
           String[] sieveNumbers = new String[options.sieveInstances];
           for (int i = 0; i < options.sieveInstances; i++) {
               sieveNumbers[i] = "ABC "+conjecture.base+"^$a$b // Sieved to 3\n";
               for (int j = 0; j < sieveCandidates.get(i).size(); j++) {
                   for (int k = conjecture.minN; k <= conjecture.step; k++) {
                       sieveNumbers[i] += k + " " + sign + sieveCandidates.get(i).get(j) + "\n";
                   }
               }
               String filePath = "temp/sr2dat"+i+".txt";
               try {
                   Files.createFile(Path.of(filePath));
               } catch (Exception e) {
                   //file already exists
               }
               FileWriter fileWriter = new FileWriter(filePath, false);
               fileWriter.write(sieveNumbers[i]);
               fileWriter.close();
           }
           Process[] sr2sieve = new Process[options.sieveInstances];
           String path = System.getProperty("user.dir") + "\\" + options.sievePath;
           for (int i = 0; i < sr2sieve.length; i++) {
               sr2sieve[i] = new ProcessBuilder().command(path, "-d", "-itemp/sr2dat"+i+".txt", "-ffact"+i+".txt","-p "+(conjecture.maxK+1),"-P "+sieveLimit).inheritIO().start();
           }
           for (Process process : sr2sieve) {
                try {
                    process.waitFor();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
           }
        }
    }
}
class Conjecture {
    String conjectureType;
    int base;
    long minK;
    long maxK;
    int startingN = 0;
    int minN;
    int maxN;
    int step = 0;
    List<Long> candidates = new ArrayList<>();
    String state = "Initialization";
    String fileName = "";
    Conjecture() {
    }
    String print() {
        return "conjectureType=" + conjectureType + "\nstate=" + state + "\nbase=" + base
                + "\nminK=" + minK + "\nmaxK=" + maxK + "\nminN=" + minN +
                "\nmaxN=" + maxN + "\nstep=" + step + "\ncandidates=" + Arrays.toString(candidates.toArray());
    }
    void saveToFile(String filePath) throws IOException {
        try {
            Files.createFile(Path.of(filePath));
        } catch (Exception e) {
            //file already exists
        }
        FileWriter fileWriter = new FileWriter(filePath, false);
        fileWriter.write(print());
        fileWriter.close();
    }
    void saveToFile() throws IOException {//Todo: add backup system
        try {
            Files.createFile(Path.of(fileName));
        } catch (Exception e) {
            //file already exists
        }
        FileWriter fileWriter = new FileWriter(fileName, false);
        fileWriter.write(print());
        fileWriter.close();
    }
    Conjecture(String inputFile) {
        String[] splitInput = inputFile.split("\n");
        for (String arg : splitInput) {
            String[] splitArg = arg.split("=");
            switch (splitArg[0]) {
                case "conjectureType" -> conjectureType = splitArg[1];
                case "state" -> state = splitArg[1];
                case "base" ->  base = Integer.parseInt(splitArg[1]);
                case "minK" -> minK = Long.parseLong(splitArg[1]);
                case "maxK" -> maxK = Long.parseLong(splitArg[1]);
                case "minN" -> minN = Integer.parseInt(splitArg[1]);
                case "maxN" -> maxN = Integer.parseInt(splitArg[1]);
                case "step" -> step = Integer.parseInt(splitArg[1]);
                case "candidates" -> {
                    String[] unfilteredArray = splitArg[1].split(",");
                    for (String s : unfilteredArray) {
                        candidates.add(Long.parseLong(s.replaceAll("[^\\d.]", "")));
                    }
                }
            }
        }
        startingN = minN;
        String fileName = conjectureType + base + "-" + minK + "-" + maxK + "-" + minN + ".ini";
    }
}
class Options {
        int pfgwInstances = 1;
        int pfgwThreadsPerInstance = 1;
        int sieveInstances = 1;
        int llrThreads = 1;
        String pfgwPath = "pfgw64.exe";
        String sievePath = "sr2sieve.exe";
        String k1b2sievePath = "k1b2sieve.exe";
        String llrPath = "llr2.exe";
        String resultFile = "DRC_results.txt";
        int stepSize = 10000;
        double sieveFactor = 10.0;
        Options(String inputFile) {
            String[] splitInput = inputFile.split("\n");
            for (String arg : splitInput) {
                String[] splitArg = arg.split("=");
                switch (splitArg[0]) {
                    case "pfgwPath" -> pfgwPath = splitArg[1];
                    case "sr2sievePath" -> sievePath = splitArg[1];
                    case "k1b2sievePath" -> k1b2sievePath = splitArg[1];
                    case "llrPath" -> llrPath = splitArg[1];
                    case "pfgwInstances" ->  pfgwInstances = Integer.parseInt(splitArg[1]);
                    case "pfgwThreadsPerInstance" ->  pfgwThreadsPerInstance = Integer.parseInt(splitArg[1]);
                    case "sieveInstances" ->  sieveInstances = Integer.parseInt(splitArg[1]);
                    case "llrThreads" ->  llrThreads = Integer.parseInt(splitArg[1]);
                    case "resultFile" ->  resultFile = splitArg[1];
                    case "stepSize" ->  stepSize = Integer.parseInt(splitArg[1]);
                    case "sieveFactor" -> sieveFactor = Double.parseDouble(splitArg[1]);
                }
            }
        }
}