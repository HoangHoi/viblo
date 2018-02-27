<?php

use Illuminate\Database\Seeder;
use Carbon\Carbon;

class UsersTableSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        $now = Carbon::now();
        DB::table('users')->truncate();
        DB::table('users')->insert([
            [
                'id' => 1,
                'name' => 'Hoang Hoi',
                'email' => 'hoanghoi@gmail.com',
                'password' => bcrypt('12344321'),
                'created_at' => $now,
                'updated_at' => $now,
            ],
        ]);
    }
}
