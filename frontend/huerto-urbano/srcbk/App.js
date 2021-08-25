import './App.css';
import '@material-ui/core';
import { Route, Switch, BrowserRouter} from "react-router-dom";
import MiniDrawer from './components/Sidebar/MiniDrawer';
import Login from './Login';
import Settings from './Settings';
import Monitor from './Monitor';
import NotFound from './NotFound';

function App() {
  return (
    <div className="app">
    <BrowserRouter>
     <MiniDrawer />
      <Switch>
        <Route exact path="/" component={Login} />
        <Route exact path="/ajustes" component={Settings} />
        <Route exact path="/medidas" component={Monitor} />
        <Route component={NotFound}/>
      </Switch>
     </BrowserRouter>
     </div>
  );
}

export default App;
