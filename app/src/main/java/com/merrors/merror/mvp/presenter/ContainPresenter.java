package com.merrors.merror.mvp.presenter;

import android.app.Application;
import android.support.v4.app.Fragment;
import android.util.SparseArray;
import android.util.SparseIntArray;
import android.util.SparseLongArray;

import com.alibaba.android.arouter.launcher.ARouter;
import com.jess.arms.integration.AppManager;
import com.jess.arms.di.scope.ActivityScope;
import com.jess.arms.mvp.BasePresenter;
import com.jess.arms.http.imageloader.ImageLoader;

import me.jessyan.rxerrorhandler.core.RxErrorHandler;

import javax.inject.Inject;

import com.merrors.merror.app.DCode;
import com.merrors.merror.app.utils.FragmentUtils;
import com.merrors.merror.mvp.contract.ContainContract;
import com.merrors.merror.mvp.ui.fragment.HomeFragment;

import java.util.ArrayList;
import java.util.List;


@ActivityScope
public class ContainPresenter extends BasePresenter<ContainContract.Model,ContainContract.View> {
    private RxErrorHandler mErrorHandler;
    private Application mApplication;
    private ImageLoader mImageLoader;
    private AppManager mAppManager;
    private SparseArray<Class<? extends Fragment>> mSparseArray;
    private ArrayList<Fragment> mFragments;

    @Inject
    public ContainPresenter(ContainContract.Model model, ContainContract.View rootView
            , RxErrorHandler handler, Application application
            , ImageLoader imageLoader, AppManager appManager) {
        super(model, rootView);
        this.mErrorHandler = handler;
        this.mApplication = application;
        this.mImageLoader = imageLoader;
        this.mAppManager = appManager;
        mSparseArray = new SparseArray<>();
        mFragments = new ArrayList<>();
    }

    private void putFragment(int flag,Class c,Fragment fragment){
        mSparseArray.put(flag,c);
        mFragments.add(fragment);
    }

    public void initFragments(){
        putFragment(DCode.FlagFragments.Flag_HomeFragment,HomeFragment.class,HomeFragment.newInstance());

        FragmentUtils.addFragments(mRootView.getSPFragmentManager(), mFragments, mRootView.getFrameID(), 0);
    }

    private Fragment getFragmentByFlag(int flag){
        return FragmentUtils.findFragment(mRootView.getSPFragmentManager(),mSparseArray.get(flag));
    }

    private Fragment getPreFragment(int flag){
        return FragmentUtils.getPreFragment(getFragmentByFlag(flag));
    }

    private List<Fragment> getFragmentsInStack(){
        return FragmentUtils.getFragmentsInStack(mRootView.getSPFragmentManager());
    }

    private List<Fragment> getFragments(){
        return FragmentUtils.getFragments(mRootView.getSPFragmentManager());
    }

    private void getFragments(int flag){
        FragmentUtils.hideAllShowFragment(getFragmentByFlag(flag));
    }

    private void popFragments(){
        FragmentUtils.popFragments(mRootView.getSPFragmentManager());
    }

    private void popToFragment(int flag,boolean haveSelf){
        FragmentUtils.popToFragment(mRootView.getSPFragmentManager(),mSparseArray.get(flag),haveSelf);
    }

    private void removeFragments(){
        FragmentUtils.removeFragments(mRootView.getSPFragmentManager());
    }

    private void removeToFragment(int flag,boolean haveSelf){
        FragmentUtils.removeToFragment(getFragmentByFlag(flag),haveSelf);
    }

    private void removeFragment(int flag){
        FragmentUtils.removeFragment(getFragmentByFlag(flag));
    }

    @Override
    public void onDestroy() {
        mSparseArray.clear();
        mFragments.clear();
        mSparseArray = null;
        mFragments = null;
        super.onDestroy();
        this.mErrorHandler = null;
        this.mAppManager = null;
        this.mImageLoader = null;
        this.mApplication = null;
    }

}
