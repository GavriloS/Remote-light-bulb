package com.gavrilo.svetlo;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import static android.content.ContentValues.TAG;

public class MainActivity extends AppCompatActivity {

    private DatabaseReference mDatabase;

    private DatabaseReference lightOne;
    private DatabaseReference lightTwo;

    int stateOne = -1;

    int stateTwo = -1;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ImageView imgLightOne = findViewById(R.id.imgLightOne);
        ImageView imgLightTwo = findViewById(R.id.imgLightTwo);

        Button btnLightOne = findViewById(R.id.btnLightOne);
        Button btnLightTwo = findViewById(R.id.btnLightTwo);


        mDatabase = FirebaseDatabase.getInstance().getReference();

        lightOne = mDatabase.child("lights").child("one");
        lightTwo = mDatabase.child("lights").child("two");
        // Firebase listener for light bulb 2
        ValueEventListener lightOneListener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                // Get current light status
                stateOne = dataSnapshot.getValue(Integer.class);
                if(stateOne == 1){
                    imgLightOne.setImageResource(R.drawable.light_on);
                }else{
                    imgLightOne.setImageResource(R.drawable.light_off);
                }
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {

                Log.w(TAG, "loadPost:onCancelled", databaseError.toException());
            }
        };
        lightOne.addValueEventListener(lightOneListener);
        // Firebase listener for light bulb 2
        ValueEventListener lightTwoListener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {

                stateTwo = dataSnapshot.getValue(Integer.class);
                if(stateTwo == 1){
                    imgLightTwo.setImageResource(R.drawable.light_on);
                }else{
                    imgLightTwo.setImageResource(R.drawable.light_off);
                }
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
                // Getting Post failed, log a message
                Log.w(TAG, "loadPost:onCancelled", databaseError.toException());
            }
        };
        lightTwo.addValueEventListener(lightTwoListener);

        //switch light status for bulb 1
        btnLightOne.setOnClickListener(v -> {
            if(stateOne == 1){
                stateOne = 0;
                lightOne.setValue(0);
            }else if(stateOne == 0){
                stateOne = 1;
                lightOne.setValue(1);
            }else{
                Toast.makeText(this, "Firebase connection failed", Toast.LENGTH_SHORT).show();
            }
        });
        // switch light status for bulb 2
        btnLightTwo.setOnClickListener(v -> {
            if(stateTwo == 1){
                stateTwo = 0;
                lightTwo.setValue(0);

            }else if (stateTwo == 0){
                stateTwo = 1;
                lightTwo.setValue(1);
            }else{
                Toast.makeText(this, "Firebase connection failed", Toast.LENGTH_SHORT).show();
            }
        });


    }
}