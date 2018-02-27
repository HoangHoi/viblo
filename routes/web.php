<?php

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', function () {
    return view('welcome');
});

Auth::routes();

Route::get('/home', 'HomeController@index')->name('home');
Route::group(['prefix' => 'device', 'namespace' => 'Device'], function () {
    Route::get('session', 'SessionController@session');
    Route::post('login', 'SessionController@login');
    Route::post('logout', 'SessionController@logout');
});
