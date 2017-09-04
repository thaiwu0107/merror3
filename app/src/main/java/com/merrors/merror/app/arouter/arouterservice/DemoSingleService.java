package com.merrors.merror.app.arouter.arouterservice;

import android.content.Context;
import android.widget.Toast;

import com.alibaba.android.arouter.facade.annotation.Route;
import com.alibaba.android.arouter.facade.template.IProvider;

/**
 * 有一些簡單的單例服務可以很簡單的用arouter來呼叫
 * Created by ggttoo44 on 2017/8/23.
 */

@Route(path = "/service/single")
public class DemoSingleService implements IProvider {
    Context mContext;

    public void sayHello(String name) {
        Toast.makeText(mContext, "Hello " + name, Toast.LENGTH_SHORT).show();
    }

    @Override
    public void init(Context context) {
        mContext = context;
    }
}
