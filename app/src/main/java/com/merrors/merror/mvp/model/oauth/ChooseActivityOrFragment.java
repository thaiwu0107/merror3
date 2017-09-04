package com.merrors.merror.mvp.model.oauth;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.github.scribejava.apis.FacebookApi;
import com.github.scribejava.core.builder.ServiceBuilder;
import com.github.scribejava.core.oauth.OAuth20Service;
import com.merrors.merror.R;

import org.fuckboilerplate.rx_social_connect.RxSocialConnect;
import org.fuckboilerplate.rx_social_connect.internal.persistence.OAuth2AccessToken;

public class ChooseActivityOrFragment extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.choose_activity_or_fragment);

        findViewById(R.id.bt_activity)
                .setOnClickListener(v -> startActivity(new Intent(this, ConnectionsActivity.class)));

        findViewById(R.id.bt_fragment)
                .setOnClickListener(v -> startActivity(new Intent(this, HostActivityFragment.class)));
    }
}
