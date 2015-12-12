function generateDummyTest() {
  var delay = 7000 + Math.random() * 7000;
  var testPassed = Math.random() > 0.5;

  return function(callback) {
    setTimeout(function() {
      callback(testPassed);
    }, delay);
  };
}

var tests = [
  { description: "commas are rotated properly",          run: generateDummyTest() },
  { description: "exclamation points stand up straight", run: generateDummyTest() },
  { description: "run-on sentences don't run forever",   run: generateDummyTest() },
  { description: "question marks curl down, not up",     run: generateDummyTest() },
  { description: "semicolons are adequately waterproof", run: generateDummyTest() },
  { description: "capital letters can do yoga",          run: generateDummyTest() }
];


var Test = React.createClass({
    render: function() {
        return (
            <tr>
                <td>{this.props.description}</td>
                <td>{this.props.status}</td>
            </tr>
        );
    }
});

var TestTable = React.createClass({
    render: function() {
        var rows = [];
        var stillRunning = 0;
        var passed = 0;
        var failed = 0;
        _.forEach(this.props.tests, function(status, description) {
            rows.push(<Test description={description} status={status} />);
            if (status == 'still running') stillRunning = stillRunning + 1;
            else if (status == 'passed') passed = passed + 1;
            else if (status == 'failed') failed = failed + 1;
        });
        var isDone = passed + failed == rows.length;
        var finished = isDone? 'FINISHED!' : '';
        return (
          <div>
            <table>
                <thead>
                    <tr>
                        <th>Description</th>
                        <th>Status</th>
                    </tr>
                </thead>
                <tbody>{rows}</tbody>
            </table>
            <div>Still running: {stillRunning}</div>
            <div>Passed: {passed}</div>
            <div>Failed: {failed}</div>
            <div>{finished}</div>
          </div>
        );
    }
});

var StartButton = React.createClass({
    render: function() {
        return (
            <button onClick={this.props.onStart}>Start Tests!</button>
        );
    }
});

var TestStatusPage = React.createClass({
    getInitialState: function() {
        var tests = {};
        _.forEach(this.props.tests, function(test){
            tests[test.description] = 'not started';
        })
        return {tests: tests};
    },
    onFinish: function(description) {
        return function(passed) {
            var tests = this.state.tests;
            tests[description] = passed? "passed" : "failed";
            this.setState({tests: tests});
        }
    },
    onStart: function() {
        var tests = {};
        _.forEach(this.state.tests, function(status, test){
            tests[test] = 'still running';
        });
        this.setState({tests: tests});
        _.forEach(this.props.tests, function(test){
            test.run(this.onFinish(test.description).bind(this));
        }.bind(this))
    },
    render: function() {
        var tests;
        return (
            <div>
                <StartButton onStart={this.onStart} />
                <TestTable tests={this.state.tests} />
            </div>
        );
    }
});


ReactDOM.render(
    <TestStatusPage tests={tests} />,
    document.getElementById('container')
);