<?php

namespace App\Http\Controllers;

use Illuminate\Http\Request;
use Tymon\JWTAuth\JWTAuth;
use Auth;

class HomeController extends Controller
{
    /**
     * Create a new controller instance.
     *
     * @return void
     */
    public function __construct()
    {
        $this->middleware('auth');
    }

    /**
     * Show the application dashboard.
     *
     * @return \Illuminate\Http\Response
     */
    public function index(JWTAuth $auth)
    {
        $jwtToken = $this->getJWTAuthToken($auth);
        return view('home', ['jwtToken' => $jwtToken]);
    }

    protected function getJWTAuthToken(JWTAuth $auth)
    {
        $user = Auth::user();
        return $auth->fromUser($user);
    }
}
