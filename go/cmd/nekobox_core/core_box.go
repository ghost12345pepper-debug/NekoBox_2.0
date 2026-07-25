package main

import (
	"context"
	"net"
	"net/http"
	"time"

	"github.com/matsuridayo/libneko/neko_common"
	"github.com/matsuridayo/libneko/neko_log"
	box "github.com/sagernet/sing-box"
	"github.com/sagernet/sing-box/common/dialer"
	"github.com/sagernet/sing/common/metadata"
)

var instance *box.Box
var instance_cancel context.CancelFunc

func setupCore() {
	//
	neko_log.SetupLog(50*1024, "./neko.log")
	//
	neko_common.GetCurrentInstance = func() interface{} {
		return instance
	}
	neko_common.DialContext = func(ctx context.Context, specifiedInstance interface{}, network, addr string) (net.Conn, error) {
		if i, ok := specifiedInstance.(*box.Box); ok {
			return dialer.NewRouter(i.Router()).DialContext(ctx, network, metadata.ParseSocksaddr(addr))
		}
		if instance != nil {
			return dialer.NewRouter(instance.Router()).DialContext(ctx, network, metadata.ParseSocksaddr(addr))
		}
		return neko_common.DialContextSystem(ctx, network, addr)
	}
	neko_common.DialUDP = func(ctx context.Context, specifiedInstance interface{}) (net.PacketConn, error) {
		if i, ok := specifiedInstance.(*box.Box); ok {
			return dialer.NewRouter(i.Router()).ListenPacket(ctx, metadata.Socksaddr{})
		}
		if instance != nil {
			return dialer.NewRouter(instance.Router()).ListenPacket(ctx, metadata.Socksaddr{})
		}
		return neko_common.DialUDPSystem(ctx)
	}
	neko_common.CreateProxyHttpClient = func(specifiedInstance interface{}) *http.Client {
		transport := &http.Transport{
			TLSHandshakeTimeout:   time.Second * 3,
			ResponseHeaderTimeout: time.Second * 3,
		}
		
		var b *box.Box
		if i, ok := specifiedInstance.(*box.Box); ok {
			b = i
		} else {
			b = instance
		}
		
		if b != nil {
			transport.DialContext = func(ctx context.Context, network, addr string) (net.Conn, error) {
				return dialer.NewRouter(b.Router()).DialContext(ctx, network, metadata.ParseSocksaddr(addr))
			}
		}

		client := &http.Client{
			Transport: transport,
		}
		return client
	}
}
