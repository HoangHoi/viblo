<?php

namespace App\Http\Controllers\Device;

use App;
use Auth;
use Illuminate\Http\Request;
use App\Http\Controllers\Controller;
use Tymon\JWTAuth\JWTAuth;
use App\Core\Responses\Auth\AuthResponse;

class SessionController extends Controller
{
    protected $guard = 'device';
    const RESPON_STATUS = [
        'not_login' => 0,
        'login_in' => 1,
        'login_fail' => 2,
        'logout_success' => 3,
    ];

    public function __construct()
    {
        $this->middleware('guest:device', ['except' => ['logout', 'session']]);
        $this->middleware('auth:device', ['only' => ['logout']]);
    }

    public function session(JWTAuth $auth)
    {
        if (Auth::guard($this->guard)->check()) {
            return $this->userWasAuthenticated($auth);
        }

        return $this->sessionResponse(self::RESPON_STATUS['not_login']);
    }

    public function login(Request $request, JWTAuth $auth)
    {
        $this->validateLogin($request);
        $credentials = $request->only('identify_code', 'password');

        if ($this->attemptLogin($request, $credentials)) {
            return $this->userWasAuthenticated($auth);
        }

        return $this->sessionResponse(self::RESPON_STATUS['login_fail']);
    }

    public function logout()
    {
        Auth::guard($this->guard)->logout();

        return $this->sessionResponse(self::RESPON_STATUS['logout_success']);
    }

    protected function attemptLogin(Request $request, $credentials)
    {
        return Auth::guard($this->guard)->attempt($credentials, true);
    }

    protected function validateLogin(Request $request)
    {
        $this->validate($request, [
            'identify_code' => 'required|string',
            'password' => 'required|string',
        ]);
    }

    protected function userWasAuthenticated($auth)
    {
        $user = Auth::guard($this->guard)->user();
        if ($user->isActive()) {
            $authToken = $auth->fromUser($user);
            $result = $this->makeUserResult($user, $authToken);

            return $this->sessionResponse(self::RESPON_STATUS['login_in'], $result);
        }

        $this->logout();
        return $this->sessionResponse(self::RESPON_STATUS['login_fail']);
    }

    protected function makeUserResult($user, $authToken)
    {
        return [
            'id' => $user->id,
            'auth_token' => $authToken,
        ];
    }

    protected function sessionResponse($status, $data = [], $statusCode = 200)
    {
        $data['status'] = $status;
        $data['token'] = csrf_token();
        return response()->json($data, $statusCode);
    }
}
