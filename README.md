<p>The goal of this repository is to create a multiprocess application in Linux using C++.</p>

<h3>Goals</h3>
<p>Simulate a lottery using 3 processors, which can be summarized as follows:</p>
<ol>
  <li>Main: This process mainly forks the other processors and handles destruction.</li>
  <li>Draw: This process draws a number between 1 and 49 randomly and sends the result to the loto process via a pipeline 'fifo1'.</li>
  <li>Loto: This process receives the numbers, displays all the number drawn every 5 seconds, and stops the application if the drawn number has already been selected.</li>
</ol>

![multiprocess](https://github.com/Gitssalah/MultiprocessingLoto/assets/144855643/596f0c69-5f88-4ea1-ba4c-8f6541c85dd9)


<h3>Strengths</h3>
<ol>
  <li>Multiprocessing programming </li>
  <li>Signal handling.</li>
  <li>Sending information using pipes 'fifo1'.</li>
  <li>Handling Linux commands and system calls.</li>
</ol>

