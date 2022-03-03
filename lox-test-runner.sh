#!/bin/bash

# Configure location for executable to be tested
EXECUTABLE="Debug/lox-cpp.exe"

function run_all_tests {
  begin_tests

  # Comment out tests to skip them
  test_print
  test_single_line_comment
  test_multi_line_comment
  test_expressions
  test_ternary
  test_logical
  test_string_concat
  test_variables
  test_scopes
  test_control
  test_break
  test_functions
  test_closure
  test_lambda
  test_classes
  test_static_method

  end_tests
}

function test_print {
  run_test 'print "Hello, world!";' 'Hello, world!'
}

function test_single_line_comment {
  run_test '
      // print "commented code";
      print "uncommented code"; // ignored
  '\
  'uncommented code'
}

function test_multi_line_comment {
  run_test '
      /*
      print "commented";
      print "code";
      */
      print /* ignored */ "uncommented code"; /* ignored */
  '\
  'uncommented code'
}

function test_expressions {
  run_test 'print 123;' '123'
  run_test 'print (-100)+(-20)+(-3);' '-123'
  run_test 'print 12.34 * 100;' '1234'
  run_test 'print "hello";' 'hello'
  run_test 'print "";' ''
  run_test 'print ((5+5)*10)/2;' '50'
  run_test 'print true;' 'true'
  run_test 'print !true;' 'false'
  
  run_test '1/0;' 'Divide by zero.\n[line 1]'
  run_test '-"muffin";' 'Operand must be a number.\n[line 1]'
}

function test_ternary {
  run_test 'print true ? "yes" : "no";' 'yes'
  run_test 'print false ? "yes" : "no";' 'no'
}

function test_logical {
  run_test 'print "hi" or 2;' 'hi'
  run_test 'print nil or "yes";' 'yes'
}

function test_string_concat {
  run_test 'print "a" + 1;' 'a1'
  run_test 'print 1 + "a";' '1a'
  run_test 'print "a" + "b";' 'ab'
  run_test 'print 1 + 1;' '2'
}

function test_variables {
  run_test 'var a=1; var b=2; var c=a+b; print c;' '3'

  run_test '
      var breakfast = "bagels";
      breakfast = "beignets";
      print breakfast;
  ' \
  'beignets'
}

function test_scopes {
  run_test '
    var a = "global a";
    var b = "global b";
    var c = "global c";
    {
      var a = "outer a";
      var b = "outer b";
      {
        var a = "inner a";
        print a;
        print b;
        print c;
      }
      print a;
      print b;
      print c;
    }
    print a;
    print b;
    print c;
  ' \
  'inner a
outer b
global c
outer a
outer b
global c
global a
global b
global c'  

  run_test '
      var volume = 11;
      volume = 0;
      {
        var volume = 3 * 4 * 5;
        print volume;
      }
  ' \
  '60'
}

function test_control {
  run_test 'if (true) { print "yes"; } else { print "no"; }' 'yes'
  run_test 'if (false) { print "yes"; } else { print "no"; }' 'no'
  run_test 'var a = 1; while (a < 10) { a = a + 1; } print a;' '10'
  run_test '
      var total = 0; 
      for (var a = 1; a < 10; a = a + 1) {
        total = total + a;
      } 
      print total;
  ' \
  '45'
}

function test_break {
  run_test 'var a = 1; while (a = 1) { break; a = 2; } print a;' '1'
}

function test_functions {
  run_test '
      fun returnSum(a, b) {
        return a + b;
      }
      var result = returnSum(1, 2);
      print result;
  ' \
  '3'
  
  run_test '
      fun count(n) {
        if (n > 1) count(n - 1);
        print n;
      }

      count(3);
  ' \
  '1\n2\n3'

  run_test 'print clock;' '<native fn>'
  run_test 'print clock() > 0;' 'true'
  run_test 'fun foo(){} print foo;' '<fn foo>'
  run_test 'fun foo(){} print foo();' 'nil'
}

function test_closure {
  run_test '
      fun returnFunction() {
        var outside = "outside";

        fun inner() {
          print outside;
        }

        return inner;
      }

      var fn = returnFunction();
      fn();
  ' \
  'outside'
  
  run_test '
      fun makeCounter() {
        var i = 0;
        fun count() {
          i = i + 1;
          print i;
        }

        return count;
      }

      var counter = makeCounter();
      counter();
      counter();
  ' \
  '1\n2'
}

function test_lambda {
  run_test 'var x = fun () { return "lambda"; }; print x();' 'lambda'
  run_test 'var x = fun () {}; print x;' '<lambda fn>'
}

function test_classes {
  run_test '
      class Foo {}
      var foo = Foo();    
      print foo;
  ' \
  'Foo instance'
  
  run_test '
      class Cake {
        taste() {
          var adjective = "delicious";
          print "The " + this.flavor + " cake is " + adjective + "!";
        }
      }

      var cake = Cake();
      cake.flavor = "German chocolate";
      cake.taste();
  ' \
  'The German chocolate cake is delicious!'
  
  run_test '
      class Breakfast {
        init(meat, bread) {
          this.meat = meat;
          this.bread = bread;
        }

        serve(who) {
          print "Enjoy your " + this.meat + " and " + this.bread + ", " + who + ".";
        }
      }

      var baconAndToast = Breakfast("bacon", "toast");
      baconAndToast.serve("Dear Reader");
  ' \
  'Enjoy your bacon and toast, Dear Reader.'
}

function test_static_method {
  run_test '
    class Math {
      class square(n) {
        return n * n;
      }
    }

    print Math.square(3);
  ' \
  '9'
}

function begin_tests {
  let COUNTER=0
  let PASSED=0
  let FAILED=0
  let START=`date +%s`
}

function end_tests {
  local end=`date +%s`
  local runtime=$((end-START))  
  echo "======"
  echo -e "Total: $COUNTER \e[32mPassed: $PASSED \e[31mFailed: $FAILED\e[0m Runtime (seconds): $runtime"
}

function watch {
  local LTIME=0
  while true
  do
    local ATIME=`stat -c %Z $EXECUTABLE`

    if [[ "$ATIME" != "$LTIME" ]]
    then    
      echo
      echo "Testing $EXECUTABLE"
      run_all_tests
      LTIME=$ATIME
    fi
    sleep 1
  done
}

function run_test {
  local input=$1
  local expect=`echo -e "$2" | tr -d "\r"`
  local actual=`echo "$input" | $EXECUTABLE 2>&1 | tr -d "\r"`
  if [ $? -ne 0 ]; then
    echo "Retry..."
    return
  fi
  if [ "$actual" = "$expect" ]; then
    let PASSED=PASSED+1
  else
    echo "Test failed for input: $input"
    echo "------"
    echo -e "\e[32mExpect: $expect\e[0m"
    echo -e "\e[31mActual: $actual\e[0m"
    let FAILED=FAILED+1
  fi
  let COUNTER=COUNTER+1
}

COUNTER=0
PASSED=0
FAILED=0
START=0

if [ "$1" = "-w" ]; then
  echo "Running in watch mode"
  watch
else
  echo "Running tests once, use '-w' flag to run in watch mode"
  run_all_tests
fi

