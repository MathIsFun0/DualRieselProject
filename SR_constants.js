const { spawn } = require('child_process');
var fs = require('fs'); //for reading and writing files
function min(x, y) {
 return Math.min(x, y) //i keep on forgetting
}
var async = require("async"); //for doing multiple sieves at once
var savename = "";
function chunkify(a, n, balanced) {
    
    if (n < 2)
        return [a];

    var len = a.length,
            out = [],
            i = 0,
            size;

    if (len % n === 0) {
        size = Math.floor(len / n);
        while (i < len) {
            out.push(a.slice(i, i += size));
        }
    }

    else if (balanced) {
        while (i < len) {
            size = Math.ceil((len - i) / n--);
            out.push(a.slice(i, i += size));
        }
    }

    else {

        n--;
        size = Math.floor(len / n);
        if (len % size === 0)
            size--;
        while (i < size * n) {
            out.push(a.slice(i, i += size));
        }
        out.push(a.slice(size * n));

    }

    return out;
} //file stolen from the internet
//1. Get a list of candidates and remove trivial mods (manually inputted)
var prompt = require('prompt') //used for user input
prompt.start()
var SR;
var base;
var minK;
var maxK;
var minN;
var maxN;
var origN;
var fatalerror = false;
var N = 0;
prompt.get(['Enter the path to the input file (leave blank for new bases)'], function (err, result) {
 var config;
 try {
 config = fs.readFileSync(result['Enter the path to the input file (leave blank for new bases)'], "utf8")
 fs.writeFileSync(result['Enter the path to the input file (leave blank for new bases)']+" (backup)", config, function(err) {})
 config = JSON.parse(config)
 savename = result['Enter the path to the input file (leave blank for new bases)']
 SR = config.SR
 base = config.base
 minN = config.currentN
 maxN = config.maxN
 if (maxN == null) {
  maxN = Infinity
 }
 minK = config.minK
 maxK = config.maxK
 candidates = config.candidates
 var status = config.status
 if (status == "Initial Testing") {
  initialTesting()
 } else if (status == "Sieving") {
  sieve(minN, config.maxS)
 } else if (status == "Testing") {
  test(config.leftToTest, config.maxTest)
 } else {
  startEntry()
 }
 } catch (err) {
  startEntry()
 }
})
function startEntry() {
prompt.get(['Sierpinski or Riesel (enter S or Ra or Rp; Ra includes negative numbers as absolute value whereas Rp excludes them)', 'Enter the base (e.g. 3)', 'Enter the minimum k', 'Enter the maximum k. (You can use the conjectured k minus 1)', 'Enter the minimum n (put 0 for new bases)', 'Enter the maximum n (put 0 for no maximum)'], function (err, result) {
    SR = result['Sierpinski or Riesel (enter S or Ra or Rp; Ra includes negative numbers as absolute value whereas Rp excludes them)']
    if (SR != "S" && SR != "Ra" && SR != "Rp") {
      fatalerror = true;
      console.log("Invalid input for Sierpinski or Riesel")
    }
    base = parseInt(result['Enter the base (e.g. 3)'])
    if (isNaN(base) || base <= 1) {
      fatalerror = true;
      console.log("Invalid input for base")
    }
    minK = parseInt(result['Enter the minimum k'])
    if (isNaN(minK) || minK <= 0) {
      fatalerror = true;
      console.log("Invalid input for minimum k")
    }
    maxK = parseInt(result['Enter the maximum k. (You can use the conjectured k minus 1)'])
    if (isNaN(maxK) || maxK < minK) {
      fatalerror = true;
      console.log("Invalid input for maximum k")
    }
    minN = parseInt(result['Enter the minimum n (put 0 for new bases)'])
    if (isNaN(minN) || minN < 0) {
      fatalerror = true;
      console.log("Invalid input for minimum n")
    }
    origN = JSON.parse(JSON.stringify(minN));
    maxN = parseInt(result['Enter the maximum n (put 0 for no maximum)'])
    if (isNaN(maxN) || maxN < 0 || (maxN > 0 && maxN < minN)) {
      fatalerror = true;
      console.log("Invalid input for maximum n")
    }
    minN++
    if (maxN == 0) {
    maxN = Infinity;
    }
    savename = SR+base+"-"+minK+"-"+maxK+".src"
 if (!fatalerror) {
  getStartingKs();
 }}
)
}
var filters = [];
function getStartingKs() {
 console.log("Creating array...");
 prompt.get(['Are there any trivial ks (based on modulos)? (y or n)'], function (err, result) {
  if (result['Are there any trivial ks (based on modulos)? (y or n)'] == "y") {
   addTrivialKs()
  } else {
   askForAlgebraicSets(false)
  }
 })
}
function addTrivialKs() {
   prompt.get(['Enter the remainder (_ mod b)', 'Enter the base (r mod _)', 'Are there any more trivial ks (based on modulos)? (y or n)'], function (err, result) {
    var rem = parseInt(result['Enter the remainder (_ mod b)']);
    var bas = parseInt(result['Enter the base (r mod _)']);
    var cont = result['Are there any more trivial ks (based on modulos)? (y or n)'];
    var isInvalid = false;
    if (isNaN(rem) || !(0 <= rem < bas)) {
     isInvalid = true;
    }
    if (isNaN(bas) || bas<=0) {
     isInvalid = true;
    }
    if (!isInvalid) {
     filters.push([rem, bas])
    }
    if (isInvalid || cont == "y") {
     if (isInvalid && cont != "y") {
      console.log("Invalid input, continued anyway");
     }
     if (isInvalid && cont == "y") {
      console.log("Invalid input");
     }
     addTrivialKs()
    } else {
     askForAlgebraicSets(false)
    }
   })
}
function askForAlgebraicSets(isStarted) {
 if (!isStarted) {
  prompt.get(['Are there any covering sets? (y or n)'], function (err, result) {
   var isSets = result['Are there any covering sets? (y or n)']
   if (isSets == "y") {
  console.log("For algebraic sets, you must enter the data as a javascript true/false statement. For example, if k makes a covering set when it equals m^2, you could enter Math.floor(Math.sqrt(k)) == Math.sqrt(k).")
    askForAlgebraicSets(true)
   } else {
    parseFilters()
   }
  })
 } else {
 prompt.get(['Enter the covering set', 'Are there any more covering sets? (y or n)'], function (err, result) {
    var set = result['Enter the covering set'];
    var cont = result['Are there any more covering sets? (y or n)'];
    filters.push(set)
    if (cont == "y") {
     askForAlgebraicSets()
    } else {
     parseFilters()
    }
   })
}}
var candidates;
function parseFilters() {
 var currentTime = Date.now()
 candidates = new Array();
 for (var k = minK; k <= maxK; k++) {
  var isPassed = true;
  for (var i = 0; i < filters.length; i++) {
  var filter = filters[i];
  if (isPassed && typeof filter == "object") {
   if (k % filter[1] == filter[0]) {
    isPassed = false;
   }
  } else if (isPassed) {
   if (eval(filter)) {
    isPassed = false;
   }
  }
  }
  if (isPassed) {
  candidates.push(k);
  }
  if (Math.floor(currentTime/1000) != Math.floor(Date.now()/1000)) {
   console.log("Creating candidates - "+(k-minK+1)+"/"+(maxK-minK+1)+", "+candidates.length+" added.")
   currentTime = Date.now()
  }
 }
 console.log(candidates.length+" ks remain before initial testing. Saving to file...");
 var fileSave = {
  SR: SR,
  base: base,
  currentN: minN,
  maxN: maxN,
  minK: minK,
  maxK: maxK,
  status: "Initial Testing",
  candidates: candidates
}
fs.writeFileSync(savename, JSON.stringify(fileSave), function(err) {})
initialTesting()
}
//2. Do initial PFGW testing (up to ~5000 digits)
function initialTesting() {
 var maxSize = Math.floor(5000 / Math.log10(base)) //maximum n under 5000 digits
 if (maxSize > maxN) {
  maxSize = maxN
 }
 if (minN > maxSize && minN != Infinity) {
  sieve(minN, min(maxN, Math.floor(minN/10000)*10000+10000))
 } else {
 if (origN == minN) {
 console.log("Testing small values - n < "+maxSize);
 }
 var candidateString = [];
 var sign;
 if (SR == "S") {
 sign = "+";
 } else {
 sign = "-";
 }
 N = minN //every time a range is complete this will increase by one
 if (candidates.length <= 128*32) {
 for (var i = 0; i < candidates.length/128; i++) {
  candidateString.push("");
  for (var j = 128*i; j < Math.min(128*i+128, candidates.length); j++) {
   var k = candidates[j];
   if (SR != "Rp" || Math.log(k)/Math.log(base)<N) {
   candidateString[i] += base + "^" + N + sign + k + "\r\n"
  }}
 }} else {
  var numPerThread = Math.floor(candidates.length/32)+1;
 for (var i = 0; i < candidates.length/numPerThread; i++) {
  candidateString.push("");
  for (var j = numPerThread*i; j < Math.min(numPerThread*i+numPerThread, candidates.length); j++) {
   var k = candidates[j];
   if (SR != "Rp" || Math.log(k)/Math.log(base)<N) {
   candidateString[i] += base + "^" + N + sign + k + "\r\n"
  }}
 }
}
  console.log("Testing n = "+N+"...");
  for (var i = 0; i < candidateString.length; i++) {
  fs.writeFileSync(SR+base+"-"+minK+"-"+maxK+"-"+N+"_"+i+".txt", candidateString[i]);
}
var numDone = 0;
var pfgw = new Array(candidateString.length)
  for (var i = 0; i < candidateString.length; i++) {
  pfgw[i] = spawn('C:/Users/user/Documents/PFGW/pfgw64 -C"quiet" -f -T4 '+SR+base+"-"+minK+"-"+maxK+"-"+N+"_"+i+".txt", [], {shell: true,detached: true}); //use factoring because no sieving
pfgw[i].stdout.on('data', (data) => {
  //process.stdout.write(data.toString());
});
  pfgw[i].on('exit', (code) => {
  numDone++;
  if (numDone == candidateString.length) {
 for (var i = 0; i < candidateString.length; i++) {
  fs.unlinkSync(SR+base+"-"+minK+"-"+maxK+"-"+N+"_"+i+".txt");
  }
if (N < maxSize) {
   N++;
   candidateString = [];
   //code to remove ks with primes
   var removed = false;
   var primes;
   try {
   primes = fs.readFileSync("pfgw-prime.log", "utf8");
   } catch (err) {
   try {
   primes = fs.readFileSync("pfgw.log", "utf8");
   } catch (err) {
   primes = ""
   }}
   try {
   primes += fs.readFileSync("pfgw.log", "utf8")
   } catch (err) {

   }
   var op = "-";
   if (SR == "S") {
    op = "+"
   }
   for (var i = candidates.length; i >= 0; i--) {
   var k = candidates[i];
   if (primes.split(base+"^"+(N-1)+op+k+"\r\n").length > 1) { //This was a BIG BUG that removed small ks for numbers that had less digits than maxK. e.g. k=1234 would get removed along with k=12345...
    candidates.splice(i, 1);
    removed = true;
   }
   }
   if (candidates.length == 0) {
var fileSave = {
  SR: SR,
  base: base,
  currentN: minN,
  maxN: maxN,
  minK: minK,
  maxK: maxK,
  status: "Done!",
  candidates: candidates
}
fs.writeFileSync(savename, JSON.stringify(fileSave), function(err) {})
   console.log("All candidates have been proven prime or PRP!")
   process.exit()
   } else if (removed) {
   console.log(candidates.length+" ks remain.")
   }
   minN = N;
   var fileSave = {
  SR: SR,
  base: base,
  currentN: minN,
  maxN: maxN,
  minK: minK,
  maxK: maxK,
  status: "Initial Testing",
  candidates: candidates
}
fs.writeFileSync(savename, JSON.stringify(fileSave), function(err) {})
   initialTesting()
  } else {
   sieve(maxSize+1, min(maxN, 10000*Math.floor(maxSize/10000)+10000));
  }
}})
 }
  }
  
}
//3. Sieve to a certain bound (removal rate based on PFGW timing test, ranges of 10000)
function sieve(minS, maxS) {
 if (maxK <= 2147483647) {
var fileSave = {
  SR: SR,
  base: base,
  currentN: minN,
  maxN: maxN,
  minK: minK,
  maxK: maxK,
  maxS: maxS,
  status: "Sieving",
  candidates: candidates
}
fs.writeFileSync(savename, JSON.stringify(fileSave), function(err) {})
 console.log("Sieving "+candidates.length+" ks from n = "+minS+" to "+maxS);
 var splitCandidates = chunkify(candidates, 3, true) //you can change 3 to something better for NewPGen to use ~100% CPU
 var numDone = 0;
 var sieves = [null, null, null]
 async.parallel({
  one: function(callback){ callback(null, sieveEach(splitCandidates[0], minS, maxS, 0, "", function(err, result) {
 sieves[0] = result;
 numDone++;
 if (numDone == 3) {
 finalizeSieve(sieves, minS, maxS)
 }}))},
  two: function(callback){ callback(null, sieveEach(splitCandidates[1], minS, maxS, 0, "", function(err, result) {
 sieves[1] = result;
 numDone++;
 if (numDone == 3) {
 finalizeSieve(sieves, minS, maxS)
 }
}))},
  three: function(callback){ callback(null, sieveEach(splitCandidates[2], minS, maxS, 0, "", function(err, result) {
 sieves[2] = result;
 numDone++;
 if (numDone == 3) {
 finalizeSieve(sieves, minS, maxS)
 }
}))}
},
function(err, results) {
})
 } else {
 console.log("Sieving impossible for large ks, skipping to test...")
 var Candidates = [];
 for (var i = 0; i < candidates.length; i++) {
  var eachK = candidates[i];
  for (var n = minS+1; n <= maxS; n++) {
   Candidates += [eachK, n]
  }
 }
 test(Candidates, maxS);
}
}
function sieveEach(cand, minS, maxS, ind, candLeft, callback) {
 if (cand) {
 var candidatesLeft = candLeft;
 var thisK = cand[ind];
 console.log("Sieving k = "+thisK+" ("+(ind+1)+"/"+cand.length+" for this thread)");
 var type;
 var operator;
 if (SR == "S") {
 type = 19;
 operator = "+";
 } else {
 type = 20;
 operator = "-";
 }
 var stdbench = "";
 var pfgwBench = spawn('C:/Users/user/Documents/PFGW/pfgw64 -Bspec,sexp='+base+'^^n'+operator+thisK+",minn="+minS+",maxn="+maxS, [], {shell:true});
pfgwBench.stdout.on('data', (data) => {
  process.stdout.write(data.toString());
  stdbench += data.toString()
});
pfgwBench.on('exit', (code) => {
var values = [];
var stdms = stdbench.split(" ms");
var stds = stdbench.split(" s");
var stdmi = stdbench.split(" mi");
var stdhr = stdbench.split(" hr");
var stdd = stdbench.split(" d");
try {
for (var i = 0; i < stdms.length-1; i++) {
 values.push(parseInt(stdms[i].split(" ")[stdms[i].split(" ").length-1])/1000)
}
for (var i = 0; i < stds.length-1; i++) {
 values.push(parseInt(stds[i].split(" ")[stdms[i].split(" ").length-1]))
}
for (var i = 0; i < stdmi.length-1; i++) {
 values.push(parseInt(stdmi[i].split(" ")[stdms[i].split(" ").length-1])*60)
}
for (var i = 0; i < stdhr.length-1; i++) {
 values.push(parseInt(stdhr[i].split(" ")[stdms[i].split(" ").length-1])*60*60)
}
for (var i = 0; i < stdd.length-1; i++) {
 values.push(parseInt(stdd[i].split(" ")[stdms[i].split(" ").length-1])*60*60*24)
}
var total = 0;
var goodVals = 0;
for(var i = 0; i < values.length; i++) {
    if (values[i]) {
    total += values[i];
    goodVals++
}}} catch (err) {
var avg = Math.floor(maxS*Math.log(base)*10000);
}
var avg = Math.floor(maxS*Math.log(base)*10000); //only 1/4 because multithreading might make sense, but there are also just as much CPU used by the sieves... and then /50 since it runs for wayyy too long
 var filename = "SieveFiles/"+SR+base+"_"+thisK+"_"+minS+"_"+maxS+".npg"
function exists(filenamee) {
try {
fs.existsSync(filename)
} catch (err) {
return false;
}
return true;
}
 if (exists(filename) && thisK != undefined) {
 console.log(avg)
 var newpgen = spawn('C:/Users/user/Documents/DualRieselPRPrimeSearch/cnewpgen.exe -wp=SieveFiles/'+SR+base+"_"+thisK+"_"+minS+"_"+maxS+".npg -t="+type+" -v -base="+base+" -k="+thisK+" -nmin="+minS+" -nmax="+maxS+" -osp="+avg, [], {shell: true});
 newpgen.stdout.on('data', (data) => {
  process.stdout.write(data.toString());
});
newpgen.on('exit', (code) => {
//Check if ind<candidates.length-1
//Get sieve result file, and add it to list
//if done, return list, otherwise add 1 to ind and continue
var sieveRESULT = "";
try {
sieveRESULT = fs.readFileSync(filename, "utf8")
} catch (err) {
sieveRESULT = "";
}
candidatesLeft += sieveRESULT;
if (ind<candidates.length-1) {
 sieveEach(cand, minS, maxS, ind+1, candidatesLeft, callback) 
} else {
callback(null, candidatesLeft)
}
})}
 else {
 //Check if ind<candidates.length-1
//Get sieve result file, and add it to list
//if done, return list, otherwise add 1 to ind and continue
var sieveRESULT = "";
try {
sieveRESULT = fs.readFileSync(filename, "utf8")
} catch (err) {
sieveRESULT = "";
}
candidatesLeft += sieveRESULT;
if (ind<cand.length-1) {
 sieveEach(cand, minS, maxS, ind+1, candidatesLeft, callback) 
} else {
callback(null, candidatesLeft)
 }
}});
} else {
 callback(null, "")
}
}
function finalizeSieve(res, minS, maxS) {
 var initialString = res[0] + "\r\n" + res[1] + "\r\n" + res[2];
 initialString = initialString.split("\r\n");
 var candidateArray = [];
 for (var i = 0; i < initialString.length; i++) {
  if (initialString[i].split(" ").length > 1) {
   candidateArray.push([parseInt(initialString[i].split(" ")[0]), parseInt(initialString[i].split(" ")[1])]);
  }
 }
 candidateArray = candidateArray.sort(function(a, b){ return a[1] - b[1] })
 console.log("Testing "+candidateArray.length+" candidates!")
 test(candidateArray, maxS)
}
//4. Test up to the max limit of the sieve.
function test(can, lim) {
var fileSave = {
  SR: SR,
  base: base,
  currentN: minN,
  maxN: maxN,
  minK: minK,
  maxK: maxK,
  maxTest: lim,
  status: "Testing",
  candidates: candidates,
  leftToTest: can
}
fs.writeFileSync(savename, JSON.stringify(fileSave), function(err) {})
 var sign;
 if (SR == "S") {
 sign = "+";
 } else {
 sign = "-";
 }
 var thisCan = base+'^'+can[0][1]+sign+can[0][0]
 var pfgw = spawn('C:/Users/user/Documents/PFGW/pfgw64 -T4 -q"'+base+'^'+can[0][1]+sign+can[0][0]+'"', [], {shell: true}); //use factoring because no sieving
  pfgw.stdout.on('data', (data) => {
  process.stdout.write(data.toString());
});
  pfgw.on('exit', (code) => {
  if (can.length > 1) {
   can.splice(0, 1)
   //code to remove ks with primes
   var removed = false;
   var primes;
   try {
   primes = fs.readFileSync("pfgw-prime.log", "utf8");
   } catch (err) {
   try {
   primes = fs.readFileSync("pfgw.log", "utf8");
   } catch (err) {
   primes = ""
   }}
   try {
   primes += fs.readFileSync("pfgw.log", "utf8")
   } catch (err) {

   }
   for (var i = candidates.length; i >= 0; i--) {
   var k = candidates[i];
 var operator;
 if (SR == "S") {
 operator = "+";
 } else {
 operator = "-";
 }
   if (primes.split(thisCan).length > 1 && k == parseInt(thisCan.split(operator)[1])) { //This was a BIG BUG that removed small ks for numbers that had less digits than maxK. e.g. k=1234 would get removed along with k=12345...
    candidates.splice(i, 1);
    for (var j = can.length-1; j >= 0; j--) {
     if (can[j][0] == k) {
      can.splice(j, 1)
     }
    }
    removed = true;
   }
   }
   if (candidates.length == 0) {
var fileSave = {
  SR: SR,
  base: base,
  currentN: minN,
  maxN: maxN,
  minK: minK,
  maxK: maxK,
  maxTest: lim,
  status: "Done!",
  candidates: candidates,
  leftToTest: can
}
fs.writeFileSync(savename, JSON.stringify(fileSave), function(err) {})
   console.log("All candidates have been proven prime or PRP!")
   process.exit()
   } else if (removed) {
   console.log(candidates.length+" ks remain.")
   }
   minN = can[0][1];
var fileSave = {
  SR: SR,
  base: base,
  currentN: minN,
  maxN: maxN,
  minK: minK,
  maxK: maxK,
  maxTest: lim,
  status: "Testing",
  candidates: candidates,
  leftToTest: can
}
fs.writeFileSync(savename, JSON.stringify(fileSave), function(err) {})
   test(can, lim)
  } else if (lim == maxN) {
   console.log("Limit reached, exiting...")
  var fileSave = {
  SR: SR,
  base: base,
  currentN: maxN,
  maxN: maxN,
  minK: minK,
  maxK: maxK,
  status: "Initial Testing",
  candidates: candidates
}
fs.writeFileSync(savename, JSON.stringify(fileSave), function(err) {})
  } else {
   sieve(lim+1, min(maxN, 10000*Math.floor(lim/10000)+10000));
  }
})
 }
//5. Get the next range and repeat until all primes are found (or infinitely)